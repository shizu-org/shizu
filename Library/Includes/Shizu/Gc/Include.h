#if !defined(SHIZU_GC_H_INCLUDED)
#define SHIZU_GC_H_INCLUDED

#include "Shizu/Runtime/Configure.h"
#include "Shizu/Cxx/Include.h"
#include <stdint.h>

typedef int32_t Shizu_Gcx_Status;
#define Shizu_Gcx_Status_Success (0)
#define Shizu_Gcx_Status_ArgumentInvalid (1)
#define Shizu_Gcx_Status_AllocationFailed (2)
#define Shizu_Gcx_Status_TypeExists (3)
#define Shizu_Gcx_Status_TypeNotExists (4)
#define Shizu_Gcx_Status_TypeUsed (5)
#define Shizu_Gcx_Status_LockFailed (6)
#define Shizu_Gcx_Status_ReferenceCounterOverflow (7)
#define Shizu_Gcx_Status_NotInitialized (8)

#define Shizu_Gcx_Status_PlsExists (9)
#define Shizu_Gcx_Status_PlsNotExists (10)

typedef void (Shizu_Gcx_VisitCallback)(void *visitContext, void *object);
typedef void (Shizu_Gcx_FinalizeCallback)(void *finalizeContext, void* object);

typedef struct Shizu_Gcx_Type Shizu_Gcx_Type;

Shizu_Gcx_Status
Shizu_Gcx_startup
  (
  );

Shizu_Gcx_Status
Shizu_Gcx_shutdown
  ( 
  );

Shizu_Gcx_Status
Shizu_Gcx_registerType
  (
    char const* name,
    size_t nameLength,
    void* visitContext,
    Shizu_Gcx_VisitCallback* visitCallback,
    void* finalizeContext,
    Shizu_Gcx_FinalizeCallback* finalizeCallback
  );

Shizu_Gcx_Status
Shizu_Gcx_unregisterType
  (
    char const* name,
    size_t nameLength
  );

Shizu_Gcx_Status
Shizu_Gcx_acquireType
  (
    char const* name,
    size_t nameLength,
    Shizu_Gcx_Type** type
  );

Shizu_Gcx_Status
Shizu_Gcx_relinquishType
  (
    Shizu_Gcx_Type* type
  );

Shizu_Gcx_Status
Shizu_Gcx_allocate
  (
    size_t size,
    Shizu_Gcx_Type* type,
    void** object
  );

Shizu_Gcx_Status
Shizu_Gcx_run
  (
    size_t* dead,
    size_t* live 
  );

void
Shizu_Gcx_visit
  (
    void* object
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Destroy process local storage.
// Shizu_Gcx_Status_ArgumentInvalid
// Shizu_Gcx_Status_PlsNotExists
Shizu_Gcx_Status
Shizu_Gcx_Pls_destroy
  (
    char const* name,
    size_t nameLength
  );

// Get if process local storage exists.
// Shizu_Gcx_Status_ArgumentValueInvalid
Shizu_Gcx_Status
Shizu_Gcx_Pls_exists
  (
    char const* name,
    size_t nameLength,
    bool* exists
  );

/// Get process local storage.
/// Shizu_Gcx_Status_PlsNotExists
/// Shizu_Gcx_Status_ArgumentInvalid
Shizu_Gcx_Status
Shizu_Gcx_Pls_get
  (
    char const* name,
    size_t nameLength,
    void **p
  );

/// Create process local storage.
/// Shizu_Gcx_Status_PlsExists
/// Shizu_Gcx_Status_ArgumentInvalid
/// Shizu_Gcx_Status_AllocationFailed
Shizu_Gcx_Status
Shizu_Gcx_Pls_create
  (
    char const* name,
    size_t nameLength,
    size_t sz,
    void **p
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // SHIZU_GCX_H_INCLUDED
