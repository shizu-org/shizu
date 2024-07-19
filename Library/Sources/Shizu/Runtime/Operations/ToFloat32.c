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
#include "Shizu/Runtime/Operations/ToFloat32.h"

#include "Shizu/Runtime/Include.h"


void
Shizu_Operations_toFloat32Version1
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (Shizu_Value_isInteger32(&argumentValues[0])) {
    Shizu_Value_setFloat32(returnValue, (Shizu_Float32)Shizu_Value_getInteger32(&argumentValues[0]));
  } else if (Shizu_Value_isFloat32(&argumentValues[0])) {
    *returnValue = argumentValues[0];
  } else if (Shizu_Runtime_Extensions_isString(state, argumentValues[0])) {
    // Not yet supported.
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
}