/*
  Shizu Runtime
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/State1.h"

/// @todo Remove this! Currently required for Shizu_OperatingSystem_* functionality.
#include "Shizu/Runtime/State.h"

#include "Shizu/Runtime/Status.h"
#include "Shizu/Runtime/DebugAssert.h"

#include "idlib/process.h"

// exit, EXIT_FAILURE
#include <stdlib.h>

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  Shizu_OperatingSystem_DlHandle
  Shizu_OperatingSystem_loadDl
    (
      char const* path
    )
  { return LoadLibrary(path); }

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  Shizu_OperatingSystem_DlHandle
  Shizu_OperatingSystem_loadDl
    (
      char const* path
    )
  {
    void* p = dlopen(path, RTLD_NOW);
    if (!p) {
      fprintf(stderr, "%s:%d: unable to load dl `%s`: reason `%s`\n", __FILE__, __LINE__, path, dlerror());
      return NULL;
    }
    return p;
  }

#else

  #error("operating system not (yet) supported")

#endif

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  void
  Shizu_OperatingSystem_unloadDl
    (
      Shizu_OperatingSystem_DlHandle handle
    )
  {
    if (Shizu_OperatingSystem_DlHandle_Invalid != handle) {
      FreeModule(handle);
    }
  }

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  void
  Shizu_OperatingSystem_unloadDl
    (
      Shizu_OperatingSystem_DlHandle handle
    )
  {
    if (Shizu_OperatingSystem_DlHandle_Invalid != handle) {
      dlclose(handle);
    }
  }

#else

  #error("operating system not (yet) supported")

#endif

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  void*
  Shizu_OperatingSystem_getDlSymbol
    (
      Shizu_OperatingSystem_DlHandle dlHandle,
      char const* symbolName
    )
  { return GetProcAddress(dlHandle, symbolName); }

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  void*
  Shizu_OperatingSystem_getDlSymbol
    (
      Shizu_OperatingSystem_DlHandle dlHandle,
      char const* symbolName
    )
  { return dlsym(dlHandle, symbolName); }

#else

  #error("operating system not (yet) supported")

#endif

struct Shizu_Dl {
  // The null pointer or the successor in the singly-linked list of DLs.
  Shizu_Dl* next;
  
  // The reference count of this DL.
  int referenceCount;
  
  // The (unique) name of the DL.
  char* name;
  
  // The fully-qualified path name of the DL.
  char* path;

  // The operating-system DL handle or Shizu_OperatingSystem_DlHandle_Invalid.
  Shizu_OperatingSystem_DlHandle handle;
};

struct Shizu_State1 {

  /// The reference count of this tier 1 state.
  int referenceCount;

  /// Reference to the process object.
  idlib_process* process;
  
  //// Pointer to the top of the jump target stack (a singly linked list of jump targets).
  Shizu_JumpTarget* jumpTargets;

  /// #Shizu_Status_NoError if no error. A non-zero Shizu_Status_* value otherwise.
  Shizu_Status status;
  
  /// `true` if exit was requested. `false` otherwise.
  /// initial value is `false`.
  bool processExitRequested;

  /// null pointer or pointer to the head of the singly-linked list of DLs.
  Shizu_Dl* dls;
  
};

#define NAME "<Shizu.State1>"

int
Shizu_State1_acquire
  (
    Shizu_State1** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  idlib_process* process = NULL;
  if (idlib_acquire_process(&process)) {
    return 1;
  }

  Shizu_State1* self = NULL;
  int result = idlib_get_global(process, NAME, strlen(NAME), (void**) & self);
  if (result != IDLIB_SUCCESS && result != IDLIB_NOT_EXISTS) {
    idlib_relinquish_process(process);
    process = NULL;
    return 1;
  }
  if (result == IDLIB_NOT_EXISTS) {
    self = malloc(sizeof(Shizu_State1));
    if (!self) {
      idlib_relinquish_process(process);
      process = NULL;
      return 1;
    }
    self->process = process;
    self->referenceCount = 0;
    
    self->dls = NULL;

    self->processExitRequested = false;
    self->status = Shizu_Status_NoError;
    self->jumpTargets = NULL;

    if (idlib_add_global(self->process, NAME, strlen(NAME), self)) {
      free(self);
      self = NULL;
      idlib_relinquish_process(process);
      process = NULL;
      return 1;
    }    
  } else {
    idlib_relinquish_process(process);
    process = NULL;
  }

  self->referenceCount++;
  *RETURN = self;
  return 0;
}

int
Shizu_State1_relinquish
  (
    Shizu_State1* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    if (self->jumpTargets) {
      fprintf(stderr, "%s:%d: warning: developer error: stack of jump targets is not empty\n", __FILE__, __LINE__);
    }
    
    if (self->dls) {
      fprintf(stderr, "%s:%d: warning: developer error: set of dynamic libraries is not empty\n", __FILE__, __LINE__);
      while (self->dls) {
        Shizu_Dl* dl = self->dls;
        self->dls = self->dls->next;
        Shizu_OperatingSystem_unloadDl(dl->handle);
        dl->handle = Shizu_OperatingSystem_DlHandle_Invalid;
        free(dl->path);
        dl->path = NULL;
        free(dl);
      }
    }
    idlib_process* process = self->process;
    self->process = NULL;
    idlib_remove_global(process, NAME, strlen(NAME));
    idlib_relinquish_process(process);
    process = NULL;
    free(self);
    self = NULL;
  }
  return 0;
}

void
Shizu_State1_pushJumpTarget
  (
    Shizu_State1* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != jumpTarget);
  jumpTarget->previous = self->jumpTargets;
  self->jumpTargets = jumpTarget;
}

void
Shizu_State1_popJumpTarget
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  self->jumpTargets = self->jumpTargets->previous;
}

Shizu_NoReturn() void
Shizu_State1_jump
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != self->jumpTargets);
  longjmp(self->jumpTargets->environment, -1);
}

void
Shizu_State1_setStatus
  (
    Shizu_State1* self,
    Shizu_Status status
  )
{
  Shizu_debugAssert(NULL != self);
  self->status = status;
}

int
Shizu_State1_getStatus
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  return self->status;
}

void
Shizu_State1_setProcessExitRequested
  (
    Shizu_State1* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  self->processExitRequested = processExitRequested;
}

bool
Shizu_State1_getProcessExitRequested
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  return self->processExitRequested;
}

Shizu_Dl*
Shizu_State1_getOrLoadDl
  (
    Shizu_State1* state,
    char const* path,
    bool load
  )
{
  Shizu_OperatingSystem_DlHandle handle = Shizu_OperatingSystem_loadDl(path);
  if (!handle) {
    return NULL;
  }
  char const* (*getDlName)() = Shizu_OperatingSystem_getDlSymbol(handle, "Shizu_getDlName");
  if (!getDlName) {
    Shizu_OperatingSystem_unloadDl(handle);
    handle = Shizu_OperatingSystem_DlHandle_Invalid;
    return NULL;
  }
  char* name1 = strdup(getDlName());
  if (!name1) {
    Shizu_OperatingSystem_unloadDl(handle);
    handle = Shizu_OperatingSystem_DlHandle_Invalid;
    return NULL;
  }
  char* path1 = NULL;

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  {
    CHAR temporary[_MAX_PATH];
    GetModuleFileName(handle, temporary, _MAX_PATH);
    path1 = strdup(temporary);
    if (!path1) {
      free(name1);
      name1 = NULL;
      Shizu_OperatingSystem_unloadDl(handle);
      handle = Shizu_OperatingSystem_DlHandle_Invalid;
      return NULL;
    }
  }

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  {
    Dl_info info;
    if (!dladdr(getDlName, &info)) {
      free(name1);
      name1 = NULL;
      Shizu_OperatingSystem_unloadDl(handle);
      handle = Shizu_OperatingSystem_DlHandle_Invalid;
      return NULL;
    }
    path1 = strdup(info.dli_fname);
    if (!path1) {
      free(name1);
      name1 = NULL;
      Shizu_OperatingSystem_unloadDl(handle);
      handle = Shizu_OperatingSystem_DlHandle_Invalid;
      return NULL;
    }
  }

#else

  #error("operating system not (yet) supported")

#endif

  Shizu_Dl* dl = state->dls;
  while (NULL != dl) {
    if (!strcmp(dl->name, name1)) {
      free(path1);
      path1 = NULL;
      free(name1);
      name1 = NULL;
      Shizu_OperatingSystem_unloadDl(handle);
      handle = NULL;
      dl->referenceCount++;
      return dl;
    }
    dl = dl->next;
  }
  if (!load) {
    free(path1);
    path1 = NULL;
    free(name1);
    name1 = NULL;
    return NULL;
  }
  dl = malloc(sizeof(Shizu_Dl));
  if (!dl) {
    free(path1);
    path1 = NULL;
    free(name1);
    name1 = NULL;
    return NULL;
  }
  dl->handle = handle;
  dl->name = name1;
  dl->path = path1;
  dl->referenceCount = 1;
  dl->next = state->dls;
  state->dls = dl;
  return dl;
}

Shizu_Dl*
Shizu_State1_getDlByName
  (
    Shizu_State1* state,
    char const* name
  )
{
  Shizu_Dl* dl = state->dls;
  while (NULL != dl) {
    if (!strcmp(dl->name, name)) {
      dl->referenceCount++;
      return dl;
    }
    dl = dl->next;
  }
  return NULL;
}

Shizu_Dl*
Shizu_State1_getDlByAdr
  (
    Shizu_State1* state,
    void* p
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  HMODULE handle = NULL;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, p, &handle);

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  void* handle = NULL;
  {
    Dl_info info;
    if (!dladdr(p, &info)) {
      Shizu_OperatingSystem_unloadDl(handle);
      handle = Shizu_OperatingSystem_DlHandle_Invalid;
      return NULL;
    }
  }

#else

  #error("operating system not (yet) supported")

#endif

  char const* (*getDlName)() = Shizu_OperatingSystem_getDlSymbol(handle, "Shizu_getDlName");
  if (!getDlName) {
    return NULL;
  }
  char const* name = getDlName();
  Shizu_Dl* dl = state->dls;
  while (NULL != dl) {
    if (!strcmp(dl->name, name)) {
      dl->referenceCount++;
      return dl;
    }
    dl = dl->next;
  }
  return NULL;    
}

void
Shizu_State1_refDl
  (
    Shizu_State1* state,
    Shizu_Dl* dl
  )
{ dl->referenceCount++; }

void
Shizu_State1_unrefDl
  (
    Shizu_State1* state,
    Shizu_Dl* dl
  )
{
  if (0 == --dl->referenceCount) {
    Shizu_Dl** previous = &state->dls;
    Shizu_Dl* current = state->dls;
    while (current) {
      if (current == dl) {
        *previous = current->next;
        current = current->next;
        Shizu_OperatingSystem_unloadDl(dl->handle);
        dl->handle = NULL;
        free(dl->name);
        dl->name = NULL;
        free(dl->path);
        dl->path = NULL;
        free(dl);
        dl = NULL;
        return;
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
  }
}

void*
Shizu_State1_getDlSymbol
  (
    Shizu_State1* state,
    Shizu_Dl* dl,
    char const* name
  )
{ return Shizu_OperatingSystem_getDlSymbol(dl->handle, name); }
