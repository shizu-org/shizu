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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/ToInteger32.h"

#include "Shizu/Runtime/Include.h"
#include <stdio.h>
#include <math.h>

void
Shizu_Operations_toInteger32Version1
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (Shizu_Value_isInteger32(&argumentValues[0])) {
    *returnValue = argumentValues[0];
  } else if (Shizu_Value_isFloat32(&argumentValues[0])) {
    float x = Shizu_Value_getFloat32(&argumentValues[0]);
    if (x != floorf(x) || x < Shizu_Integer32_Minimum || x > Shizu_Integer32_Maximum) {
      Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
      Shizu_State2_jump(state);
    } else {
      *returnValue = (Shizu_Value)Shizu_Value_InitializerInteger32(x);
    }
  } else if (Shizu_Runtime_Extensions_isString(state, argumentValues[0])) {
    // Not yet supported.
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
}
