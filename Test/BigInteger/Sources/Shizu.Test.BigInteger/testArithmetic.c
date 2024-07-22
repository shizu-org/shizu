/*
  Shizu
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

#include "testArithmetic.h"

void
Shizu_Test_BigInteger_testNegate
  (
    Shizu_State2* state
  )
{
  Shizu_Value received, expected;
  Shizu_BigInteger* x = NULL, * y = NULL;

  x = Shizu_BigInteger_create(state);
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, INT32_C(0));
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, Shizu_Integer32_Minimum);
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "2147483648", sizeof("2147483648") - 1));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  x = Shizu_BigInteger_createFromInteger32(state, Shizu_Integer32_Maximum);
  y = Shizu_BigInteger_negate(state, x);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "-2147483647", sizeof("-2147483647") - 1));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}

void
Shizu_Test_BigInteger_testAddition
  (
    Shizu_State2* state
  )
{
  Shizu_String* s;
  Shizu_Value received, expected;
  Shizu_BigInteger* x = NULL, * y = NULL, * z = NULL;

  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_add(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_add(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_add(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "7", strlen("7")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_add(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "12", strlen("12"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "2147483648", strlen("2147483648")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_add(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "2147483648", strlen("2147483648"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}

void
Shizu_Test_BigInteger_testSubtraction
  (
    Shizu_State2* state
  )
{
  Shizu_String* s;
  Shizu_Value received, expected;
  Shizu_BigInteger* x = NULL, * y = NULL, * z = NULL;

  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "-5", strlen("-5"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "5", strlen("5")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "7", strlen("7")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "-2", strlen("-2"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "2147483648", strlen("2147483648")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "2147483648", strlen("2147483648"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  //
  x = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  y = Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "2147483648", strlen("2147483648")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_subtract(state, x, y));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_createFromString(state, Shizu_String_create(state, "-2147483648", strlen("-2147483648"))));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}
