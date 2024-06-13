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

/* Test store and load of scalar values (Shizu_Boolean, Shizu_Float32, Shizu_Integer32, Shizu_Void) in/from Shizu_Value. */
static void
test1
  (
    Shizu_State* state
  );

/* Getting the Shizu_ByteArray, Shizu_List, Shizu_Map, Shizu_Object, Shizu_String, and Shizu_WeakReference types. */
static void
test2
  (
    Shizu_State* state
  );

/* Creating Shizu_ByteArray, Shizu_List, Shizu_Map, Shizu_String, and Shizu_WeakReference objects. */
static void
test3
  (
    Shizu_State* state
  );

static void
test1
  (
    Shizu_State* state
  )
{

  Shizu_Value value;

  /* Shizu_Boolean. */
  Shizu_Value_setBoolean(&value, Shizu_Boolean_True);
  if (!Shizu_Value_isBoolean(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Boolean_True != Shizu_Value_getBoolean(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  Shizu_Value_setBoolean(&value, Shizu_Boolean_False);
  if (!Shizu_Value_isBoolean(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Boolean_False != Shizu_Value_getBoolean(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  /* Shizu_Float32 */
  Shizu_Value_setFloat32(&value, 0.f);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (0.f != Shizu_Value_getFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value_setFloat32(&value, Shizu_Float32_Maximum);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Float32_Maximum != Shizu_Value_getFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value_setFloat32(&value, Shizu_Float32_Minimum);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Float32_Minimum != Shizu_Value_getFloat32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  /* Shizu_Integer32 */
  Shizu_Value_setInteger32(&value, 0);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (0 != Shizu_Value_getInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value_setInteger32(&value, Shizu_Integer32_Maximum);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Integer32_Maximum != Shizu_Value_getInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value_setInteger32(&value, Shizu_Integer32_Minimum);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (Shizu_Integer32_Minimum != Shizu_Value_getInteger32(&value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}

static void
test2
  (
    Shizu_State* state
  )
{ 
  Shizu_ByteArray_getType(state);
  Shizu_List_getType(state);
  Shizu_Map_getType(state);
  Shizu_Object_getType(state);
  Shizu_String_getType(state);
  Shizu_WeakReference_getType(state);
}

static void
test3
  (
    Shizu_State* state
  )
{
  Shizu_ByteArray_create(state);
  Shizu_List_create(state);
  Shizu_Map_create(state);
  Shizu_String_create(state, "Hello, World!", sizeof("Hello, World!") - 1);
  Shizu_WeakReference_create(state, (Shizu_Object*)NULL);
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
  if (safeExecute(&test3)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
