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

#include "Shizu.Test.BigInteger2/testType.h"

void
Shizu_Test_BigInteger2_testType
  (
    Shizu_State2* state
  )
{
  Shizu_Type* objectType = Shizu_Object_getType(state);
  if (!objectType) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Type* bigIntegerType = Shizu_BigInteger2_getType(state);
  if (!bigIntegerType) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isTrueSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), bigIntegerType, objectType)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
}
