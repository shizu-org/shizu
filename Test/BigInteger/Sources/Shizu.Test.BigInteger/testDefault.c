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

#include "testDefault.h"

void
Shizu_Test_BigInteger_testDefault
  (
    Shizu_State2* state
  )
{
  Shizu_Value received, expected;
  Shizu_BigInteger* x = NULL;

  x = Shizu_BigInteger_create(state);
  received = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_BigInteger_toString(state, x));
  expected = (Shizu_Value)Shizu_Value_InitializerObject(Shizu_String_create(state, "0", sizeof("0") - 1));
  if (!Shizu_Value_isEqualTo(state, &expected, &received)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}
