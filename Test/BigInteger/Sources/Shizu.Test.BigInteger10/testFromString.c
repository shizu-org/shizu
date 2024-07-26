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

#include "Shizu.Test.BigInteger10/testFromString.h"

void
Shizu_Test_BigInteger10_testFromString
  (
    Shizu_State2* state
  )
{
  Shizu_Value received, expected;
  Shizu_BigInteger10* x = NULL;

  x = Shizu_BigInteger10_createFromString(state, Shizu_String_create(state, "0", strlen("0")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger10_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", strlen("0")));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  // Integer32.Minimum
  x = Shizu_BigInteger10_createFromString(state, Shizu_String_create(state, "-2147483648", strlen("-2147483648")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger10_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "-2147483648", strlen("-2147483648")));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  // Integer32.Maximum
  x = Shizu_BigInteger10_createFromString(state, Shizu_String_create(state, "2147483647", strlen("2147483647")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger10_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "2147483647", strlen("2147483647")));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

#if 1 == Shizu_Configuration_WithInteger64

  // Integer64.Minimum
  x = Shizu_BigInteger10_createFromString(state, Shizu_String_create(state, "-9223372036854775808", strlen("-9223372036854775808")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger10_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "-9223372036854775808", strlen("-9223372036854775808")));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

  // Integer64.Maximum
  x = Shizu_BigInteger10_createFromString(state, Shizu_String_create(state, "9223372036854775807", strlen("9223372036854775807")));
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger10_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "9223372036854775807", strlen("9223372036854775807")));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }

#endif

}
