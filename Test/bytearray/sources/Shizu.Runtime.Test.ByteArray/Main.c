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

#include "Shizu/Runtime/Include.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// fprintf, stdout, stderr
#include <stdio.h>
// strlen
#include <string.h>

/* Get the Shizu_ByteArray type. Test its relation to the Shizu_Object type. */
static void
test1
  (
    Shizu_State* state
  );

/* Create a Shizu_ByteArray object. */
static void
test2
  (
    Shizu_State* state
  );

static void
test1
  (
    Shizu_State* state
  )
{
  Shizu_Type* objectType = Shizu_Object_getType(state);
  if (!objectType) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  Shizu_Type* byteArrayType = Shizu_ByteArray_getType(state);
  if (!byteArrayType) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  if (!Shizu_Type_isTrueSubTypeOf(state, byteArrayType, objectType)) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
}

static void
test2
  (
    Shizu_State* state
  )
{ 
  Shizu_ByteArray* byteArray = Shizu_ByteArray_create(state);
  if (!byteArray) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  Shizu_Value v = Shizu_ByteArray_getSize(state, byteArray);
  if (!Shizu_Value_isInteger32(&v)) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  if (Shizu_Value_getInteger32(&v)) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
}

static int
safeExecute
  (
    void (*test)(Shizu_State* state)
  )
{
  if (!test) {
    return 1;
  }
  Shizu_State* state = NULL;
  if (Shizu_State_create(&state)) {
    return 1;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State_ensureModulesLoaded(state);
    (*test)(state);
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_destroy(state);
    state = NULL;
    return 1;
  }
  Shizu_State_destroy(state);
  state = NULL;
  return 0;
}

int
main
  (
    int argc,
    char** argv
  )
{
  bool failed = false;
  if (safeExecute(&test1)) {
    failed = true;
  }
  if (safeExecute(&test2)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
