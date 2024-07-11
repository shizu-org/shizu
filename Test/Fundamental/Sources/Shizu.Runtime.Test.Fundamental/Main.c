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
    Shizu_State2* state
  );

/* Getting the Shizu_ByteArray, Shizu_List, Shizu_Map, Shizu_Object, Shizu_String, and Shizu_WeakReference types. */
static void
test2
  (
    Shizu_State2* state
  );

/* Creating Shizu_ByteArray, Shizu_List, Shizu_Map, Shizu_String, and Shizu_WeakReference objects. */
static void
test3
  (
    Shizu_State2* state
  );

/* Proper behavior of integer addition, subtraction, multiplication, addition. */
static void
test4
  (
    Shizu_State2* state
  );

static void
test1
  (
    Shizu_State2* state
  )
{

  Shizu_Value value;

  /* Shizu_Boolean. */
  Shizu_Value_setBoolean(&value, Shizu_Boolean_True);
  if (!Shizu_Value_isBoolean(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Boolean_True != Shizu_Value_getBoolean(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }

  Shizu_Value_setBoolean(&value, Shizu_Boolean_False);
  if (!Shizu_Value_isBoolean(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Boolean_False != Shizu_Value_getBoolean(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }

  /* Shizu_Float32 */
  Shizu_Value_setFloat32(&value, 0.f);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (0.f != Shizu_Value_getFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setFloat32(&value, Shizu_Float32_Maximum);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Float32_Maximum != Shizu_Value_getFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setFloat32(&value, Shizu_Float32_Minimum);
  if (!Shizu_Value_isFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Float32_Minimum != Shizu_Value_getFloat32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }

  /* Shizu_Integer32 */
  Shizu_Value_setInteger32(&value, 0);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (0 != Shizu_Value_getInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setInteger32(&value, Shizu_Integer32_Maximum);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Integer32_Maximum != Shizu_Value_getInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setInteger32(&value, Shizu_Integer32_Minimum);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (Shizu_Integer32_Minimum != Shizu_Value_getInteger32(&value)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
}

static void
test2
  (
    Shizu_State2* state
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
    Shizu_State2* state
  )
{
  Shizu_ByteArray_create(state);
  {
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
    Shizu_Value_setType(&argumentValues[0], Shizu_Environment_getType(state));
    Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  }
  {
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
    Shizu_Value_setType(&argumentValues[0], Shizu_List_getType(state));
    Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  }
  {
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
    Shizu_Value_setType(&argumentValues[0], Shizu_Map_getType(state));
    Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  }
  {
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() };
    Shizu_Value_setType(&argumentValues[0], Shizu_WeakReference_getType(state));
    Shizu_Value_setVoid(&argumentValues[1], Shizu_Void_Void);
    Shizu_Operations_create(state, &returnValue, 2, &argumentValues[0]);
  }
  Shizu_String_create(state, "Hello, World!", sizeof("Hello, World!") - 1);
}

static void
test4
  (
    Shizu_State2* state
  )
{
#define ADD(x,y,z) \
  { \
    Shizu_Value returnValue = Shizu_Value_Initializer(); \
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() }; \
    Shizu_Value_setInteger32(&argumentValues[0], (x)); \
    Shizu_Value_setInteger32(&argumentValues[1], (y)); \
    Shizu_Operations_add_i32(state, &returnValue, 2, &argumentValues[0]); \
    if (!Shizu_Value_isInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
    if ((z) != Shizu_Value_getInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
  }

#define SUBTRACT(x,y,z) \
  { \
    Shizu_Value returnValue = Shizu_Value_Initializer(); \
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() }; \
    Shizu_Value_setInteger32(&argumentValues[0], (x)); \
    Shizu_Value_setInteger32(&argumentValues[1], (y)); \
    Shizu_Operations_subtract_i32(state, &returnValue, 2, &argumentValues[0]); \
    if (!Shizu_Value_isInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
    if ((z) != Shizu_Value_getInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
  }

#define MULTIPLY(x,y,z) \
  { \
    Shizu_Value returnValue = Shizu_Value_Initializer(); \
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() }; \
    Shizu_Value_setInteger32(&argumentValues[0], (x)); \
    Shizu_Value_setInteger32(&argumentValues[1], (y)); \
    Shizu_Operations_multiply_i32(state, &returnValue, 2, &argumentValues[0]); \
    if (!Shizu_Value_isInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
    if ((z) != Shizu_Value_getInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
  }

#define DIVIDE(x,y,z) \
  { \
    Shizu_Value returnValue = Shizu_Value_Initializer(); \
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() }; \
    Shizu_Value_setInteger32(&argumentValues[0], (x)); \
    Shizu_Value_setInteger32(&argumentValues[1], (y)); \
    Shizu_Operations_divide_i32(state, &returnValue, 2, &argumentValues[0]); \
    if (!Shizu_Value_isInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
    if ((z) != Shizu_Value_getInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed); \
      Shizu_State2_jump(state); \
    } \
  }


  ADD(0, 0, 0);
  ADD(1, 0, 1);
  ADD(0, 1, 1);

  ADD(Shizu_Integer32_Maximum, 0, Shizu_Integer32_Maximum);
  ADD(0, Shizu_Integer32_Maximum, Shizu_Integer32_Maximum);
  ADD(Shizu_Integer32_Maximum, 1, Shizu_Integer32_Minimum);
  ADD(1, Shizu_Integer32_Maximum, Shizu_Integer32_Minimum);
  ADD(Shizu_Integer32_Maximum, 2, Shizu_Integer32_Minimum + 1);
  ADD(2, Shizu_Integer32_Maximum, Shizu_Integer32_Minimum + 1);
  ADD(Shizu_Integer32_Maximum, Shizu_Integer32_Maximum, Shizu_Integer32_Minimum + Shizu_Integer32_Maximum - 1);
  ADD(Shizu_Integer32_Maximum, Shizu_Integer32_Maximum, Shizu_Integer32_Minimum + Shizu_Integer32_Maximum - 1);

  ADD(Shizu_Integer32_Minimum, Shizu_Integer32_Minimum, 0);
  ADD(Shizu_Integer32_Minimum, Shizu_Integer32_Minimum, 0);

  SUBTRACT(Shizu_Integer32_Minimum, 0, Shizu_Integer32_Minimum);
  SUBTRACT(0, Shizu_Integer32_Minimum, Shizu_Integer32_Minimum);
  SUBTRACT(Shizu_Integer32_Minimum, 1, Shizu_Integer32_Maximum);
  SUBTRACT(1, Shizu_Integer32_Maximum, -(Shizu_Integer32_Maximum-1));

  MULTIPLY(Shizu_Integer32_Minimum, 0, 0);
  MULTIPLY(0, Shizu_Integer32_Minimum, 0);
  MULTIPLY(Shizu_Integer32_Maximum, 0, 0);
  MULTIPLY(0, Shizu_Integer32_Maximum, 0);
  MULTIPLY(Shizu_Integer32_Minimum, 1, Shizu_Integer32_Minimum);
  MULTIPLY(1, Shizu_Integer32_Minimum, Shizu_Integer32_Minimum);
  MULTIPLY(Shizu_Integer32_Maximum, 1, Shizu_Integer32_Maximum);
  MULTIPLY(1, Shizu_Integer32_Maximum, Shizu_Integer32_Maximum);

  Shizu_Integer32 x = Shizu_Integer32_Minimum + 1;
  do {
    if (x % 2 == 0) {
      MULTIPLY(Shizu_Integer32_Maximum, x, -x); // -x = -MAX cannot overflow as |MIN|=|MAX|+1
                                                // -x = -MIN can overflow. However, x is never MIN.
      MULTIPLY(x, Shizu_Integer32_Maximum, -x);
      // Basically if you add the minimum to the minimum you end up
      // at 0. If you a
      MULTIPLY(Shizu_Integer32_Minimum, x, 0);
      MULTIPLY(x, Shizu_Integer32_Minimum, 0);
    } else {
      MULTIPLY(Shizu_Integer32_Maximum, x, Shizu_Integer32_Maximum - (x - 1));
      MULTIPLY(x, Shizu_Integer32_Maximum, Shizu_Integer32_Maximum - (x - 1));
      MULTIPLY(Shizu_Integer32_Minimum, x, Shizu_Integer32_Minimum);
      MULTIPLY(x, Shizu_Integer32_Minimum, Shizu_Integer32_Minimum);
    }
    if (x == Shizu_Integer32_Maximum) {
      break;
    } else {
      x++;
    }
  } while (true);

  DIVIDE(x,1,x);
  DIVIDE(1,2,0);

#undef DIVIDE
#undef MULTIPLY
#undef SUBTRACT
#undef ADD
}

static int
safeExecute
  (
    void (*test)(Shizu_State2* state)
  )
{
  if (!test) {
    return 1;
  }
  Shizu_State2* state = NULL;
  if (Shizu_State2_acquire(&state)) {
    return 1;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State2_ensureModulesLoaded(state);
    (*test)(state);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_relinquish(state);
    state = NULL;
    return 1;
  }
  Shizu_State2_relinquish(state);
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
  if (safeExecute(&test4)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
