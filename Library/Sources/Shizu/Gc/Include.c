#include "Shizu/Gc/Include.h"

#include "Shizu/Cxx/Include.h"
#include <malloc.h>
#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct PlsNode PlsNode;
typedef struct PlsManager PlsManager;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Tag Tag;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct TypeNode TypeNode;
typedef struct TypeManager TypeManager;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Tag_Flags_White (1)

#define Tag_Flags_Black (2)

#define Tag_Flags_Gray (Tag_Flags_White | Tag_Flags_Black)

struct Tag {
  // We could compress "flags" and "type" into 8 Byte.
  // We can use the upper 2^62 Bit for the pointer and the lower 2 bit for the color.
  void* flags;
  union {
    Tag* gray;
    TypeNode* type;
  };
  Tag* next;
};

/// @since 1.0
/// @brief Get if an Tag is colored white.
/// @param object A pointer to the Tag.
/// @return @a true if the Tag is colored white. @a false otherwise.
static inline bool
Tag_isWhite
  (
    Tag const* tag
  )
{ return Tag_Flags_White == (Tag_Flags_Gray & (uintptr_t)tag->flags); }

/// @since 1.0
/// @internal
/// @brief Color a Tag white.
/// @param tag A pointer to the Tag.
static inline void
Tag_setWhite
  (
    Tag* tag
  )
{ tag->flags = (void*)((~Tag_Flags_Black & (uintptr_t)tag->flags) | Tag_Flags_White); }

/// @since 1.0
/// @brief Get if an Tag is colored gray.
/// @param object A pointer to the Tag.
/// @return @a true if the Tag is colored gray. @a false otherwise.
static inline bool
Tag_isGray
  (
    Tag const* tag
  )
{ return Tag_Flags_Gray == (Tag_Flags_Gray & (uintptr_t)tag->flags); }

/// @since 1.0
/// @internal
/// @brief Color a Tag gray.
/// @param tag A pointer to the Tag.
static inline void
Tag_setGray
  (
    Tag* tag
  )
{ tag->flags = (void*)(uintptr_t)Tag_Flags_Gray; }

/// @since 1.0
/// @brief Get if an Tag is colored black.
/// @param object A pointer to the Tag.
/// @return @a true if the Tag is colored black. @a false otherwise.
static inline bool
Tag_isBlack
  (
    Tag const* tag
  )
{ return Tag_Flags_Black == (Tag_Flags_Gray & (uintptr_t)tag->flags); }

/// @since 1.0
/// @internal
/// @brief Color a Tag black.
/// @param tag A pointer to the Tag.
static inline void
Tag_setBlack
  (
    Tag* tag
  )
{ tag->flags = (void*)(uintptr_t)((~Tag_Flags_White & (uintptr_t)tag->flags) |  Tag_Flags_Black); }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct PlsNode {
  PlsNode* next;
  char* name;
  size_t nameLength;
  size_t hashValue;
  size_t sz;
  void* p;
};

struct PlsManager {
  PlsNode** p;
  size_t sz, cp;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct TypeNode {
  TypeNode* next;
  uint64_t usage;
  char *name;
  size_t nameLength;
  size_t hashValue;

  void* visitContext;
  Shizu_Gcx_VisitCallback* visitCallback;
  void* finalizeContext;
  Shizu_Gcx_FinalizeCallback* finalizeCallback;

  Tag* gray; // A list only used during marking.
  Tag* all; // All objects of this type.
};

struct TypeManager {
  TypeNode** p;
  size_t sz, cp;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Singleton {
  PlsManager* plsManager;
  TypeManager* typeManager;
  int64_t referenceCount;
} Singleton;

static Singleton* g_singleton = NULL;

#if (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Linux)  || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Cygwin) || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Macos)

  #include <pthread.h>

  // The system will free this automatically when the process exits.
  static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

  static Shizu_Gcx_Status
  _getSingletonVar
    (
      Singleton*** singleton
    )
  {
    *singleton = &g_singleton;
    return Shizu_Gcx_Status_Success;
  }

  static Shizu_Gcx_Status
  _lockImpl
    (
    )
  {
    if (pthread_mutex_lock(&g_lock)) {
      return Shizu_Gcx_Status_LockFailed;
    }
    return Shizu_Gcx_Status_Success;
  }

  static Shizu_Gcx_Status
  _unlockImpl
    (
    )
  {
    if (pthread_mutex_unlock(&g_lock)) {
      return Shizu_Gcx_Status_LockFailed;
    }
    return Shizu_Gcx_Status_Success;
  }

#elif Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  // The system will free this automatically when the process exits.
  static volatile HANDLE g_lock = NULL;

  __declspec(dllexport) Shizu_Gcx_Status
  _getSingletonVar
    (
      Singleton*** singleton
    )
  {
    *singleton = &g_singleton;
    return Shizu_Gcx_Status_Success;
  }

  __declspec(dllexport) Shizu_Gcx_Status
  _lockImpl
    (
    )
  {
    if (!InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL)) {
      HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
      if (!mutex) {
        return Shizu_Gcx_Status_LockFailed;
      }
      if (NULL != InterlockedCompareExchangePointer((volatile void*)&g_lock, (void*)mutex, NULL)) {
        CloseHandle(mutex);
        mutex = NULL;
      }
    }
    if (WAIT_FAILED == WaitForSingleObject(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL), INFINITE)) {
      return Shizu_Gcx_Status_LockFailed;
    }
    return Shizu_Gcx_Status_Success;
  }

  __declspec(dllexport) Shizu_Gcx_Status
  _unlockImpl
    (
    )
  {
    ReleaseMutex(InterlockedCompareExchangePointer((volatile void*)&g_lock, NULL, NULL));
    return Shizu_Gcx_Status_Success;
  }

#else

  #error("operating system not (yet) supported")

#endif

static Shizu_Gcx_Status
hashName
  (
    char const* name,
    size_t nameLength,
    size_t* hashValue
  );

static Shizu_Gcx_Status
startupPlsManager
  (
  );

static Shizu_Gcx_Status
shutdownPlsManager
  (
  );

static Shizu_Gcx_Status
startupTypeManager
  (
  );

static Shizu_Gcx_Status
shutdownTypeManager
  (
  );

static Shizu_Gcx_Status
getSingletonVar
  (
    Singleton*** singleton
  );

static Shizu_Gcx_Status
lockMutex
  ( 
  );

static Shizu_Gcx_Status
unlockMutex
  (
  );

static Shizu_Gcx_Status
hashName
  (
    char const* name,
    size_t nameLength,
    size_t* hashValue
  )
{
  size_t hv = nameLength;
  for (size_t i = 0; i < nameLength; ++i) {
    hv = hv * 37 + (size_t)name[i];
  }
  *hashValue = hv;
  return Shizu_Gcx_Status_Success;
}

static Shizu_Gcx_Status
startupPlsManager
  (
  )
{
  Singleton** singleton = NULL;
  Shizu_Gcx_Status status;
  status = getSingletonVar(&singleton);
  if (status) {
    return status;
  }
  (*singleton)->plsManager = malloc(sizeof(PlsManager));
  if (!(*singleton)->plsManager) {
    fprintf(stdout, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(PlsManager));
    return Shizu_Gcx_Status_AllocationFailed;
  }
  (*singleton)->plsManager->p = malloc(sizeof(PlsNode*) * 8);
  if (!(*singleton)->plsManager->p) {
    fprintf(stdout, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(PlsNode*) * 8);
    free((*singleton)->plsManager);
    (*singleton)->plsManager = NULL;
    return Shizu_Gcx_Status_AllocationFailed;
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    (*singleton)->plsManager->p[i] = NULL;
  }
  (*singleton)->plsManager->sz = 0;
  (*singleton)->plsManager->cp = 8;
  return Shizu_Gcx_Status_Success;
}

static Shizu_Gcx_Status
shutdownPlsManager
  (
  )
{
  Singleton** singleton = NULL;
  Shizu_Gcx_Status status;
  status = getSingletonVar(&singleton);
  if (status) {
    return status;
  }
  for (size_t i = 0, n = (*singleton)->plsManager->cp; i < n; ++i) {
    PlsNode** previous = &((*singleton)->plsManager->p[i]);
    PlsNode* current = (*singleton)->plsManager->p[i];
    while (current) {
      //if (!current->usage) {
        PlsNode* node = current;
        *previous = current->next;
        current = current->next;
        free(node->name);
        free(node);
        (*singleton)->plsManager->sz--;
      //} else {
      //previous = &(current->next);
      //current = current->next;
      //}
    }
  }
  if ((*singleton)->plsManager->sz > 0) {
    return Shizu_Gcx_Status_TypeExists;
  }
  free((*singleton)->plsManager->p);
  free((*singleton)->plsManager);
  (*singleton)->plsManager = NULL;
  return Shizu_Gcx_Status_Success;
}

static Shizu_Gcx_Status
startupTypeManager
  (
  )
{
  Singleton** singleton = NULL;
  Shizu_Gcx_Status status;
  status = getSingletonVar(&singleton);
  if (status) {
    return status;
  }
  (*singleton)->typeManager = malloc(sizeof(TypeManager));
  if (!(*singleton)->typeManager) {
    fprintf(stdout, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(TypeManager));
    return Shizu_Gcx_Status_AllocationFailed;
  }
  (*singleton)->typeManager->p = malloc(sizeof(TypeNode*) * 8);
  if (!(*singleton)->typeManager->p) {
    fprintf(stdout, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(TypeNode*) * 8);
    free((*singleton)->typeManager);
    (*singleton)->typeManager = NULL;
    return Shizu_Gcx_Status_AllocationFailed;
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    (*singleton)->typeManager->p[i] = NULL;
  }
  (*singleton)->typeManager->sz = 0;
  (*singleton)->typeManager->cp = 8;
  return Shizu_Gcx_Status_Success;
}

static Shizu_Gcx_Status
shutdownTypeManager
  (
  )
{
  Singleton** singleton = NULL;
  Shizu_Gcx_Status status;
  status = getSingletonVar(&singleton);
  if (status) {
    return status;
  }
  for (size_t i = 0, n = (*singleton)->typeManager->cp; i < n; ++i) {
    TypeNode** previous = &((*singleton)->typeManager->p[i]);
    TypeNode* current = (*singleton)->typeManager->p[i];
    while (current) {
      if (!current->usage) {
        TypeNode* node = current;
        *previous = current->next;
        current = current->next;
        free(node->name);
        free(node);
        (*singleton)->typeManager->sz--;
      } else {
        previous = &(current->next);
        current = current->next;
      }
    }
  }
  if ((*singleton)->typeManager->sz > 0) {
    return Shizu_Gcx_Status_TypeExists;
  }
  free((*singleton)->typeManager->p);
  free((*singleton)->typeManager);
  (*singleton)->typeManager = NULL;
  return Shizu_Gcx_Status_Success;
}

static Shizu_Gcx_Status
lockMutex
  (
  )
{
#if (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Linux)  || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Cygwin) || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Macos)

  return _lockImpl();

#elif Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Windows

  // We use GetModuleHandle(NULL) for that we always use the same instance of lockImpl that always uses the same instance of g_lock.
  HMODULE module = GetModuleHandle(NULL);
  if (!module) {
    return Shizu_Gcx_Status_LockFailed;
  }
  Shizu_Gcx_Status(*f)() = (Shizu_Gcx_Status(*)())GetProcAddress(module, "_lockImpl");
  if (!f) {
    return Shizu_Gcx_Status_LockFailed;
  }
  return (*f)();

#else

  #error("operating system not (yet) supported")

#endif
}

static Shizu_Gcx_Status
unlockMutex
  (
  )
{
#if (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Linux)  || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Cygwin) || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Macos)

  return _unlockImpl();

#elif Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Windows

  // We use GetModuleHandle(NULL) for that we always use the same instance of unlockImpl that always uses the same instance of g_lock.
  HMODULE module = GetModuleHandle(NULL);
  if (!module) {
    return Shizu_Gcx_Status_LockFailed;
  }
  Shizu_Gcx_Status(*f)() = (Shizu_Gcx_Status(*)())GetProcAddress(module, "_unlockImpl");
  if (!f) {
    return Shizu_Gcx_Status_LockFailed;
  }
  return (*f)();

#else

  #error("operating system not (yet) supported")

#endif
}

static Shizu_Gcx_Status
getSingletonVar
  (
    Singleton*** singleton
  )
{
#if (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Linux)  || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Cygwin) || \
    (Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Macos)

  return _getSingletonVar(singleton);

#elif Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Windows

  // We use GetModuleHandle(NULL) for that we always use the same instance of unlockImpl that always uses the same instance of g_lock.
  HMODULE module = GetModuleHandle(NULL);
  if (!module) {
    return Shizu_Gcx_Status_LockFailed;
  }
  Shizu_Gcx_Status(*f)() = (Shizu_Gcx_Status(*)())GetProcAddress(module, "_getSingletonVar");
  if (!f) {
    return Shizu_Gcx_Status_LockFailed;
  }
  return (*f)(singleton);

#else

  #error("operating system not (yet) supported")

#endif
}

Shizu_Gcx_Status
Shizu_Gcx_startup
  (
  )
{
  Shizu_Gcx_Status status;
  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }
  if (!(*singleton)) {
    (*singleton) = malloc(sizeof(Singleton));
    if (!(*singleton)) {
      unlockMutex();
      return Shizu_Gcx_Status_AllocationFailed;
    }
    (*singleton)->referenceCount = 0;
    (*singleton)->typeManager = NULL;
  }
  if (0 == (*singleton)->referenceCount) {
    status = startupTypeManager();
    if (status) {
      unlockMutex();
      return status;
    }
    status = startupPlsManager();
    if (status) {
      shutdownTypeManager();
      unlockMutex();
      return status;
    }
    (*singleton)->referenceCount++;
    unlockMutex();
    return Shizu_Gcx_Status_Success;
  } else if (INT64_MAX == (*singleton)->referenceCount) {
    unlockMutex();
    return Shizu_Gcx_Status_ReferenceCounterOverflow;
  } else {
    (*singleton)->referenceCount++;
    unlockMutex();
    return Shizu_Gcx_Status_Success;
  }
}

Shizu_Gcx_Status
Shizu_Gcx_shutdown
  ( 
  )
{
  Shizu_Gcx_Status status;

  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }
  if (1 == (*singleton)->referenceCount) {
    status = shutdownPlsManager();
    if (status) {
      /*We have to ignore this. Must not be reached. Proof of non-reachability required.*/
    }
    status = shutdownTypeManager();
    if (status) {
      /*We have to ignore this. Must not be reached. Proof of non-reachability required.*/
    }
    free(*singleton);
    (*singleton) = NULL;
    unlockMutex();
    return Shizu_Gcx_Status_Success;
  } else if (INT64_MIN == g_singleton->referenceCount) {
    unlockMutex();
    status = Shizu_Gcx_Status_ReferenceCounterOverflow;
    return status;
  } else {
    (*singleton)->referenceCount--; 
    unlockMutex();
    return Shizu_Gcx_Status_Success;
  }
}

Shizu_Gcx_Status
Shizu_Gcx_registerType
  (
    char const* name,
    size_t nameLength,
    void* visitContext,
    Shizu_Gcx_VisitCallback* visitCallback,
    void* finalizeContext,
    Shizu_Gcx_FinalizeCallback* finalizeCallback
  )
{
  Shizu_Gcx_Status status;

  if (!name || !visitCallback) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }

  status = lockMutex();
  if (status) {
    return status;
  }

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }
 
  if (!(*singleton)->typeManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->typeManager->cp;
  
  for (TypeNode* node = (*singleton)->typeManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        unlockMutex();
        status = Shizu_Gcx_Status_TypeExists;
        return status;
      }
    }
  }

  // Too many types?
  if ((*singleton)->typeManager->sz == SIZE_MAX / sizeof(TypeNode*)) {
    unlockMutex();
    return Shizu_Gcx_Status_AllocationFailed;
  }

  TypeNode* node = malloc(sizeof(TypeNode));
  if (!node) {
    unlockMutex();
    return Shizu_Gcx_Status_AllocationFailed;
  }

  node->name = malloc(nameLength > 0 ? nameLength : 1);
  if (!node->name) {
    free(node);
    unlockMutex();
    return Shizu_Gcx_Status_AllocationFailed;
  }
  memcpy(node->name, name, nameLength);
  node->nameLength = nameLength;
  node->hashValue = hashValue;

  node->visitContext = visitContext;
  node->visitCallback = visitCallback;

  node->finalizeContext = finalizeContext;
  node->finalizeCallback = finalizeCallback;

  node->usage = 1;

  node->all = NULL;
  node->gray = NULL;

  node->next = (*singleton)->typeManager->p[hashIndex];
  (*singleton)->typeManager->p[hashIndex] = node;
  (*singleton)->typeManager->sz++;
  
  unlockMutex();
  return Shizu_Gcx_Status_Success;
}

Shizu_Gcx_Status
Shizu_Gcx_unregisterType
  (
    char const* name,
    size_t nameLength
  )  
{
  if (!name) {
    return Shizu_Gcx_Status_ArgumentInvalid;
  }

  Shizu_Gcx_Status status;

  status = lockMutex();
  if (status) {
    return status;
  }

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->typeManager->cp;

  for (TypeNode* node = (*singleton)->typeManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        if (!node->usage) {
          unlockMutex();
          status = Shizu_Gcx_Status_ReferenceCounterOverflow;
          return status;
        } else {
          node->usage--;
          unlockMutex();
          status = Shizu_Gcx_Status_TypeUsed;
          return status;
        }
      }
    }
  }
  unlockMutex();
  status = Shizu_Gcx_Status_TypeNotExists;
  return status;
}

Shizu_Gcx_Status
Shizu_Gcx_acquireType
  (
    char const* name,
    size_t nameLength,
    Shizu_Gcx_Type** type
  )
{
  Shizu_Gcx_Status status;

  if (!name) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }

  status = lockMutex();
  if (status) {
    return status;
  }

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->typeManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->typeManager->cp;

  for (TypeNode* node = (*singleton)->typeManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        if (INT64_MAX == node->usage) {
          unlockMutex();
          status = Shizu_Gcx_Status_ReferenceCounterOverflow;
          return status;
        } else {
          node->usage++;
        }
        *type = (Shizu_Gcx_Type*)node;
        unlockMutex();
        return Shizu_Gcx_Status_Success;
      }
    }
  }
  unlockMutex();
  return Shizu_Gcx_Status_TypeNotExists;
}

Shizu_Gcx_Status
Shizu_Gcx_relinquishType
  (
    Shizu_Gcx_Type* type
  )
{
  Shizu_Gcx_Status status;

  if (!type) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }

  status = lockMutex();
  if (status) {
    return status;
  }

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->typeManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }
  if (0 == ((TypeNode*)type)->usage) {
    unlockMutex();
    status = Shizu_Gcx_Status_ReferenceCounterOverflow;
    return status;
  }
  ((TypeNode*)type)->usage--;

  unlockMutex();
  return Shizu_Gcx_Status_Success;
}

Shizu_Gcx_Status
Shizu_Gcx_allocate
  (
    size_t size,
    Shizu_Gcx_Type* type,
    void** object
  )
{
  Shizu_Gcx_Status status;

  if (!type || !object) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }

  status = lockMutex();
  if (status) {
    return status;
  }

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->typeManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }
  if (SIZE_MAX - sizeof(Tag) < size) {
    unlockMutex();
    status = Shizu_Gcx_Status_AllocationFailed;
    return status;
  }
  Tag* tag = malloc(sizeof(Tag) + size);
  if (!tag) {
    unlockMutex();
    status = Shizu_Gcx_Status_AllocationFailed;
    return status;
  }
  TypeNode* typeNode = (TypeNode*)type;
  if (INT64_MAX == typeNode->usage) {
    unlockMutex();
    free(tag);
    status = Shizu_Gcx_Status_ReferenceCounterOverflow;
    return status;
  }
  typeNode->usage++;
  tag->flags = 0;
  Tag_setWhite(tag);
  tag->type = typeNode;
  tag->next = typeNode->all;
  typeNode->all = tag;
  //tag->gray = NULL;
  *object = (void*)(tag + 1);
  return Shizu_Gcx_Status_Success;
}

Shizu_Gcx_Status
Shizu_Gcx_run
  (
    size_t* dead,
    size_t* live
  )
{
  Shizu_Gcx_Status status;
  size_t dead1 = 0;
  size_t live1 = 0;

  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  for (size_t i = 0, n = (*singleton)->typeManager->cp; i < n; ++i) {
    TypeNode* node = (*singleton)->typeManager->p[i];
    while (node) {
      while (node->gray) {
        // Dequeue from gray list.
        Tag* tag = node->gray;
        node->gray = tag->gray;
        // Restore the type pointer.
        tag->type = node;
        // Mark the object as black.
        Tag_setBlack(tag);
        // Note: The type should not be in the gray list if it has no visit function.
        if (node->visitCallback) {
          node->visitCallback(tag->type->visitContext, (void*)(tag + 1));
        }
      }
      node = node->next;
    }
  }
  for (size_t i = 0; i < (*singleton)->typeManager->cp; ++i) {
    TypeNode* node = (*singleton)->typeManager->p[i];
    while (node) {
      Tag** previous = &(node->all);
      Tag* current = node->all;
      while (current) {
        if (Tag_isWhite(current)) {
          Tag* tag = current;
          *previous = current->next;
          current = current->next;
          if (tag->type->finalizeCallback) {
            tag->type->finalizeCallback(tag->type->finalizeContext, (void*)(tag + 1));
          }
          tag->type->usage--;
          free(tag);
          dead1++;
        } else {
          Tag_setWhite(current);
          previous = &current->next;
          current = current->next;
          live1++;
        }
      }
      node = node->next;
    }
  }
  *dead = dead1;
  *live = live1;
  return Shizu_Gcx_Status_Success;
}

void
Shizu_Gcx_visit
  (
    void* object
  )
{
  Tag* tag = ((Tag*)object) - 1;
  if (Tag_isWhite(tag)) {
    TypeNode* type = tag->type;
    Shizu_Cxx_Debug_assert(NULL != type);
    tag->gray = type->gray;
    type->gray = tag;
    Tag_setGray(tag);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Shizu_Gcx_Status
Shizu_Gcx_Pls_destroy
  (
    char const* name,
    size_t nameLength
  )
{
  Shizu_Gcx_Status status;
  if (!name) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }
  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->plsManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->plsManager->cp;

  PlsNode** previous = &((*singleton)->plsManager->p[hashIndex]);
  PlsNode* current = (*singleton)->plsManager->p[hashIndex];
  while (current) {
    if (current->hashValue == hashValue && current->nameLength == nameLength) {
      if (!memcmp(current->name, name, nameLength)) {
        *previous = current->next;
        free(current->p);
        free(current->name);
        free(current);
        (*singleton)->plsManager->sz--;
        unlockMutex();
        return Shizu_Gcx_Status_Success;
      }
    }
    previous = &current->next;
    current = current->next;
  }

  unlockMutex();
  return Shizu_Gcx_Status_PlsNotExists;
}

Shizu_Gcx_Status
Shizu_Gcx_Pls_exists
  (
    char const* name,
    size_t nameLength,
    bool* exists
  )
{
  Shizu_Gcx_Status status;
  if (!name || !exists) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }
  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->plsManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->plsManager->cp;

  for (PlsNode* node = (*singleton)->plsManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        *exists = true;
        unlockMutex();
        return Shizu_Gcx_Status_Success;
      }
    }
  }
  *exists = false;
  unlockMutex();
  return Shizu_Gcx_Status_Success;
}

Shizu_Gcx_Status
Shizu_Gcx_Pls_get
  (
    char const* name,
    size_t nameLength,
    void** p
  )
{
  Shizu_Gcx_Status status;
  if (!name || !p) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }
  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->plsManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->plsManager->cp;

  for (PlsNode* node = (*singleton)->plsManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        *p = node->p;
        unlockMutex();
        return Shizu_Gcx_Status_Success;
      }
    }
  }
  unlockMutex();
  return Shizu_Gcx_Status_PlsNotExists;
}

Shizu_Gcx_Status
Shizu_Gcx_Pls_create
  (
    char const* name,
    size_t nameLength,
    size_t sz,
    void** p
  )
{
  Shizu_Gcx_Status status;
  if (!name || !p) {
    status = Shizu_Gcx_Status_ArgumentInvalid;
    return status;
  }
  status = lockMutex();
  if (status) {
    return status;
  }
  Singleton** singleton = NULL;
  status = getSingletonVar(&singleton);
  if (status) {
    unlockMutex();
    return status;
  }

  if (!(*singleton)->plsManager) {
    unlockMutex();
    status = Shizu_Gcx_Status_NotInitialized;
    return status;
  }

  size_t hashValue;
  status = hashName(name, nameLength, &hashValue);
  if (status) {
    unlockMutex();
    return status;
  }

  size_t hashIndex = hashValue % (*singleton)->plsManager->cp;

  for (PlsNode* node = (*singleton)->plsManager->p[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue && node->nameLength == nameLength) {
      if (!memcmp(node->name, name, nameLength)) {
        unlockMutex();
        return Shizu_Gcx_Status_PlsExists;
      }
    }
  }

  PlsNode* node = malloc(sizeof(PlsNode));
  if (!node) {
    unlockMutex();
    return Shizu_Gcx_Status_AllocationFailed;
  }
  node->name = malloc(nameLength > 0 ? nameLength : 1);
  if (!node->name) {
    unlockMutex();
    free(node);
    return Shizu_Gcx_Status_AllocationFailed;
  }
  memcpy(node->name, name, nameLength);
  node->nameLength = nameLength;
  node->hashValue = hashValue;

  node->sz = sz;
  node->p = malloc(sz > 0 ? sz : 1);
  if (!node->p) {
    unlockMutex();
    free(node->name);
    free(node);
    return Shizu_Gcx_Status_AllocationFailed;
  }

  node->next = (*singleton)->plsManager->p[hashIndex];
  (*singleton)->plsManager->p[hashIndex] = node;
  (*singleton)->plsManager->sz++;
  
  unlockMutex();
  return Shizu_Gcx_Status_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
