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
#include "Shizu/Runtime/State2.h"

#include <string.h>
#include "idlib/process.h"
#include "Shizu/Runtime/DebugAssert.h"
#include "Shizu/Runtime/State1.h"

#include "Shizu/Runtime/Locks.private.h"
#include "Shizu/Runtime/Type.private.h"

struct Shizu_State2 {
  int referenceCount;
  Shizu_State1* state1;
  idlib_process* process;

  Shizu_Gc* gc;
  Shizu_Locks* locks;
  Shizu_Types* types;
};

static int
Shizu_State2_create
  (
    Shizu_State2** RETURN
  );

static int
Shizu_State2_destroy
  (
    Shizu_State2* self
  );

static int
Shizu_State2_create
  (
    Shizu_State2** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  Shizu_State2* self = malloc(sizeof(Shizu_State2));
  if (!self) {
    return 1;
  }

  self->process = NULL;
  self->state1 = NULL;
  
  self->gc = NULL;
  self->locks = NULL;
  self->types = NULL;
  if (Shizu_State1_acquire(&self->state1)) {
    free(self);
    self = NULL;
    return 1;
  }
  if (idlib_process_acquire(&self->process)) {
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    free(self);
    self = NULL;
    return 1;
  }
  Shizu_JumpTarget jumpTarget;
  //
  Shizu_State1_pushJumpTarget(self->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    self->types = Shizu_Types_startup(self->state1);
    Shizu_State1_popJumpTarget(self->state1);
  } else {
    Shizu_State1_popJumpTarget(self->state1);
    idlib_process_relinquish(self->process);
    self->process = NULL;
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    free(self);
    self = NULL;
    return 1;
  }
  Shizu_State1_pushJumpTarget(self->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    self->gc = Shizu_Gc_startup(self->state1);
    Shizu_State1_popJumpTarget(self->state1);
  } else {
    Shizu_State1_popJumpTarget(self->state1);
    Shizu_Types_shutdown(self->state1, self->types);
    self->types = NULL;
    idlib_process_relinquish(self->process);
    self->process = NULL;
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    free(self);
    self = NULL;
    return 1;
  }
  Shizu_State1_pushJumpTarget(self->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    self->locks = Shizu_Locks_startup(self->state1);
    Shizu_State1_popJumpTarget(self->state1);
  } else {
    Shizu_State1_popJumpTarget(self->state1);
    Shizu_Gc_shutdown(self->state1, self->gc);
    self->gc = NULL;
    Shizu_Types_shutdown(self->state1, self->types);
    self->types = NULL;
    idlib_process_relinquish(self->process);
    self->process = NULL;
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    free(self);
    self = NULL;
    return 1;
  }
   *RETURN = self;
  return 0;
}

static int
Shizu_State2_destroy
  (
    Shizu_State2* self
  )
{
  Shizu_Locks_shutdown(self->state1, self->locks);
  self->locks = NULL;
  Shizu_Gc_shutdown(self->state1, self->gc);
  self->gc = NULL;
  Shizu_Types_shutdown(self->state1, self->types);
  self->types =NULL;
  idlib_process_relinquish(self->process);
  self->process = NULL;
  Shizu_State1_relinquish(self->state1);
  self->state1 = NULL;
  free(self);
  self = NULL;
  return 0;
}

#define NAME "<Shizu.State2>"

int
Shizu_State2_acquire
  (
    Shizu_State2** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  idlib_process* process = NULL;
  if (idlib_process_acquire(&process)) {
    return 1;
  }
  Shizu_State2* self = NULL;
  int result = idlib_get_global(process, NAME, strlen(NAME), (void**)&self);
  if (result != IDLIB_SUCCESS && result != IDLIB_NOT_EXISTS) {
    idlib_process_relinquish(process);
    process = NULL;
    return 1;
  }
  if (result == IDLIB_NOT_EXISTS) {
    if (Shizu_State2_create(&self)) {
      idlib_process_relinquish(process);
      process = NULL;
      return 1;
    }
    self->referenceCount = 0;
    if (idlib_add_global(self->process, NAME, strlen(NAME), self)) {
      free(self);
      self = NULL;
      idlib_process_relinquish(process);
      process = NULL;
      return 1;
    }
  } else {
    idlib_process_relinquish(process);
    process = NULL;
  }
  self->referenceCount++;
  *RETURN = self;
  return 0;
}

int
Shizu_State2_relinquish
  (
    Shizu_State2* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    idlib_process* process = NULL;
    idlib_process_acquire(&process);
    Shizu_State2_destroy(self);
    self = NULL;
    idlib_remove_global(process, NAME, strlen(NAME));
    idlib_process_relinquish(process);
    process = NULL;
  }
  return 0;
}

void
Shizu_State2_pushJumpTarget
  (
    Shizu_State2* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != self->state1);
  Shizu_debugAssert(NULL != jumpTarget);
  Shizu_State1_pushJumpTarget(self->state1, jumpTarget);
}

void
Shizu_State2_popJumpTarget
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_popJumpTarget(self->state1);
}

Shizu_NoReturn() void
Shizu_State2_jump
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_jump(self->state1);
}

void
Shizu_State2_setStatus
  (
    Shizu_State2* self,
    Shizu_Status status
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setStatus(self->state1, status);
}

int
Shizu_State2_getStatus
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getStatus(self->state1);
}

void
Shizu_State2_setProcessExitRequested
  (
    Shizu_State2* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setProcessExitRequested(self->state1, processExitRequested);
}

bool
Shizu_State2_getProcessExitRequested
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getProcessExitRequested(self->state1);
}

Shizu_State1*
Shizu_State2_getState1
  (
    Shizu_State2* self
  )
{ return self->state1; }

Shizu_Types*
Shizu_State2_getTypes
  (
    Shizu_State2* self
  )
{ return self->types; }

Shizu_Gc*
Shizu_State2_getGc
  (
    Shizu_State2* self
  )
{ return self->gc; }

Shizu_Locks*
Shizu_State2_getLocks
  (
    Shizu_State2* self
  )
{ return self->locks;}
