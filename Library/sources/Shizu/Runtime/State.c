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
#include "Shizu/Runtime/State.h"

#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Configure.h"
#include "Shizu/Runtime/Gc.private.h"
#include "Shizu/Runtime/Locks.private.h"
#include "Shizu/Runtime/Stack.private.h"
#include "Shizu/Runtime/Objects/WeakReference.private.h"
#include "Shizu/Runtime/Type.private.h"
#include "Shizu/Runtime/DebugAssert.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// strcmp
#include <string.h>

#include "idlib/process.h"

typedef struct NamedStorageNode NamedStorageNode;

struct NamedStorageNode {
  NamedStorageNode* next;
  char* name;
  void* p;
  size_t n;
};

/// @brief Replacement for C global variables / singletons.
typedef struct NamedStorageService {
  NamedStorageNode* nodes;
} NamedStorageService;

int
NamedStorageService_initialize
  (
    NamedStorageService* self
  )
{
  self->nodes = NULL;
  return 0;
}

int
NamedStorageService_uninitialize
  (
    NamedStorageService* self
  )
{
  if (self->nodes) {
    fprintf(stderr, "%s:%d: warning: storage service still contains storage nodes\n", __FILE__, __LINE__);
  }
  return 0;
}

int
NamedStorageService_allocate
  (
    NamedStorageService* self,
    char const* name,
    size_t n
  )
{
  NamedStorageNode* node = NULL;

  node = self->nodes;
  while (node) {
    if (!strcmp(node->name, name)) {
      return 1;
    }
    node = node->next;
  }

  node = malloc(sizeof(NamedStorageNode));
  if (!node) {
    return 1;
  }
  node->name = strdup(name);
  if (!node->name) {
    free(node);
    return 1;
  }
  node->p = malloc(n > 0 ? n : 1);
  if (!node->p) {
    free(node->name);
    free(node);
    return 1;
  }
  node->n = n;
  node->next = self->nodes;
  self->nodes = node;
  return 0;
}

int
NamedStorageService_deallocate
  (
    NamedStorageService* self,
    char const* name
  )
{
  NamedStorageNode** previous = &self->nodes;
  NamedStorageNode* current = self->nodes;
  while (current) {
    if (!strcmp(current->name, name)) {
      NamedStorageNode* node = current;
      *previous = current->next;
      current = current->next;
      free(node->name);
      node->name = NULL;
      free(node->p);
      node->p = NULL;
      free(node);
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  return 0;
}

int
NamedStorageService_get
  (
    NamedStorageService* self,
    char const* name,
    void** p
  )
{
  NamedStorageNode* current = self->nodes;
  while (current) {
    if (!strcmp(current->name, name)) {
      *p = current->p;
      return 0;
    } else {
      current = current->next;
    }
  }
  return 1;
}



struct Shizu_State {
  int referenceCount;
  Shizu_State1* state1;
  NamedStorageService namedStorageService;
  Shizu_Gc* gc;
  Shizu_Locks* locks;
  Shizu_Stack* stack;
  Shizu_Types types;
};

static Shizu_State* g_singleton = NULL;

#define NAME "Shizu.State.Singleton"

int
Shizu_State_create
  (
    Shizu_State** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  if (!g_singleton) {
    Shizu_State* self = malloc(sizeof(Shizu_State));
    if (!self) {
      return 1;
    }
    if (Shizu_State1_acquire(&self->state1)) {
      free(self);
      self = NULL;
      return 1;
    }
    if (NamedStorageService_initialize(&self->namedStorageService)) {
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    self->gc = NULL;
    self->locks = NULL;
    self->stack = NULL;

    Shizu_JumpTarget jumpTarget;
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->gc = Shizu_Gc_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->locks = Shizu_Locks_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->stack = Shizu_Stack_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Locks_shutdown(self, self->locks);
      self->locks = NULL;
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Types_initialize(self->state1, &self->types);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Stack_shutdown(self, self->stack);
      self->stack = NULL;
      Shizu_Locks_shutdown(self, self->locks);
      self->locks = NULL;
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    self->referenceCount = 0;
    g_singleton = self;
  }
  g_singleton->referenceCount++;
  *RETURN = g_singleton;
  return 0;
}

void
Shizu_Types_uninitialize
  (
    Shizu_State* self
  )
{
  while (self->types.size) {
    // Worst-case is k^n where n is the number of types and k is the capacity.
    // Do *no*u use this in performance critical code.
    for (size_t i = 0, n = self->types.capacity; i < n; ++i) {
      Shizu_Type** previous = &(self->types.elements[i]);
      Shizu_Type* current = self->types.elements[i];
      while (current) {
        if (!Shizu_Types_getTypeChildCount(self->state1, &self->types, current)) {
          Shizu_Type* type = current;
          Shizu_Types_ensureDispatchUninitialized(self->state1, &self->types, type);
          *previous = current->next;
          current = current->next;
          self->types.size--;
          if (type->typeDestroyed) {
            type->typeDestroyed(self->state1);
          }
          Shizu_Type_destroy(self, &self->types, type);
        } else {
          previous = &current->next;
          current = current->next;
        }
      }
    }
  }
}

int
Shizu_State_destroy
  (
    Shizu_State* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    Shizu_Gc_run(self);
    Shizu_Stack_shutdown(self, self->stack);
    self->stack = NULL;
    Shizu_Locks_shutdown(self, self->locks);
    self->locks = NULL;
    Shizu_Gc_shutdown(self, self->gc);
    self->gc = NULL;
    Shizu_Types_uninitialize(self);
    NamedStorageService_uninitialize(&self->namedStorageService);
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    g_singleton = NULL;
    free(self);
  }
  return 0;
}

void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_pushJumpTarget(self->state1, jumpTarget);
}

void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_popJumpTarget(self->state1);
}

Shizu_NoReturn() void
Shizu_State_jump
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_jump(self->state1);
}

void
Shizu_State_setStatus
  (
    Shizu_State* self,
    Shizu_Status error
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setStatus(self->state1, error);
}

Shizu_Status
Shizu_State_getStatus
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getStatus(self->state1);
}

void
Shizu_State_setProcessExitRequested
  (
    Shizu_State* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setProcessExitRequested(self->state1, processExitRequested);
}

bool
Shizu_State_getProcessExitRequested
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getProcessExitRequested(self->state1);
}

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* self,
    char const* name
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != name);
  return Shizu_Types_getTypeByName(self->state1, &self->types, name);
}

Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* typeDescriptor
  )
{
  Shizu_Type* type = Shizu_Types_createType(self->state1, &self->types, name, parentType, dl, typeDestroyed, typeDescriptor);
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(self->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    if (type->descriptor->staticInitialize) {
      type->descriptor->staticInitialize(self);
    }
    type->flags |= Shizu_TypeFlags_StaticallyInitialized;
    Shizu_Types_ensureDispatchInitialized(self->state1, &self->types, type);
  }
  Shizu_State1_popJumpTarget(self->state1);
  return type;
}

Shizu_Gc*
Shizu_State_getGc
  (
    Shizu_State* self
  )
{
  return self->gc;
}

Shizu_Locks*
Shizu_State_getLocks
  (
    Shizu_State* self
  )
{ return self->locks;}

Shizu_Stack*
Shizu_State_getStack
  (
    Shizu_State* self
  )
{ return self->stack; }

Shizu_Types*
Shizu_State_getTypes
  (
    Shizu_State* self
  )
{ return &self->types; }

int
Shizu_State_allocateNamedMemory
  (
    Shizu_State* state,
    char const* name,
    size_t n
  )
{ return NamedStorageService_allocate(&state->namedStorageService, name, n); }

int
Shizu_State_deallocateNamedMemory
  (
    Shizu_State* state,
    char const* name
  )
{ return NamedStorageService_deallocate(&state->namedStorageService, name); }

int
Shizu_State_getNamedMemory
  (
    Shizu_State* state,
    char const* name,
    void** p
  )
{ return NamedStorageService_get(&state->namedStorageService, name, p); }

Shizu_Dl*
Shizu_State_getOrLoadDl
  (
    Shizu_State* state,
    char const* path,
    bool load
  )
{ return Shizu_State1_getOrLoadDl(state->state1, path, load); }

Shizu_Dl*
Shizu_State_getDlByName
  (
    Shizu_State* state,
    char const* name
  )
{ return Shizu_State1_getDlByName(state->state1, name); }

Shizu_Dl*
Shizu_State_getDlByAdr
  (
    Shizu_State* state,
    void *p
  )
{ return Shizu_State1_getDlByAdr(state->state1, p); }

void
Shizu_Dl_ref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  )
{ Shizu_State1_refDl(state->state1, dl); }

void
Shizu_Dl_unref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  )
{ Shizu_State1_unrefDl(state->state1, dl); }

void*
Shizu_Dl_getSymbol
  (
    Shizu_State* state,
    Shizu_Dl* dl,
    char const* name
  )
{ return Shizu_State1_getDlSymbol(state->state1, dl, name); }
