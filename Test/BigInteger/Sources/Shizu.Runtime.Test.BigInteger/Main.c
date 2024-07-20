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

/* Get the Shizu_BigInteger type. Test its relation to the Shizu_Object type. */
static void
test1
  (
    Shizu_State2* state
  );

/* Create a Shizu_BigInteger with the default constructor. */
static void
test2
  (
    Shizu_State2* state
  );

static void
test1
  (
    Shizu_State2* state
  )
{
  Shizu_Type* objectType = Shizu_Object_getType(state);
  if (!objectType) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Type* bigIntegerType = Shizu_BigInteger_getType(state);
  if (!bigIntegerType) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isTrueSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), bigIntegerType, objectType)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}

static void
test2
  (
    Shizu_State2* state
  )
{ 
  Shizu_Value received, expected;
  Shizu_BigInteger* bigInteger = NULL;

  bigInteger = Shizu_BigInteger_create(state);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, bigInteger));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  bigInteger = Shizu_BigInteger_createFromInteger32(state, INT32_C(0));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, bigInteger));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  bigInteger = Shizu_BigInteger_createFromInteger32(state, INT32_C(Shizu_Integer32_Minimum));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, bigInteger));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "-2147483648", sizeof("-2147483648") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  
  bigInteger = Shizu_BigInteger_createFromInteger32(state, INT32_C(Shizu_Integer32_Maximum));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, bigInteger));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "2147483647", sizeof("2147483647") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}

static void
test3
  (
    Shizu_State2* state
  )
{
  Shizu_Value received, expected;
  Shizu_BigInteger* x = NULL, *y = NULL;

  x = Shizu_BigInteger_create(state);
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, INT32_C(0));
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, INT32_C(Shizu_Integer32_Minimum));
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "2147483648", sizeof("2147483648") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, INT32_C(Shizu_Integer32_Maximum));
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "-2147483647", sizeof("-2147483647") - 1));
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)Shizu_Value_getObject(&expected), &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
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
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
