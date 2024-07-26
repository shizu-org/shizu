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
#include "Shizu/Runtime/Operations/ToInteger.h"

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
    return;
  }
#if 1 == Shizu_Configuration_WithInteger64
  if (Shizu_Value_isInteger64(&argumentValues[0])) {
    Shizu_Integer64 v = Shizu_Value_getInteger64(&argumentValues[0]);
    if (v < Shizu_Integer32_Minimum || v > Shizu_Integer32_Maximum) {
      Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
      Shizu_State2_jump(state);
    }
    Shizu_Value_setInteger32(returnValue, (Shizu_Integer32)v);
    return; 
  }
#endif
  if (Shizu_Value_isFloat32(&argumentValues[0])) {
    float x = Shizu_Value_getFloat32(&argumentValues[0]);
    if (x != floorf(x) || x < Shizu_Integer32_Minimum || x > Shizu_Integer32_Maximum) {
      Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
      Shizu_State2_jump(state);
    } else {
      Shizu_Value_setInteger32(returnValue, (Shizu_Integer32)x);
    }
  } else if (Shizu_Runtime_Extensions_isString(state, argumentValues[0])) {
    Shizu_BigInteger10* bigInteger = Shizu_BigInteger10_createFromString(state, (Shizu_String*)Shizu_Value_getObject(&argumentValues[0]));
    Shizu_Value_setInteger32(returnValue, Shizu_BigInteger10_toInteger32(state, bigInteger));
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
}

#if 1 == Shizu_Configuration_WithInteger64

void
Shizu_Operations_toInteger64Version1
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (Shizu_Value_isInteger32(&argumentValues[0])) {
    Shizu_Value_setInteger64(returnValue, Shizu_Value_getInteger32(&argumentValues[0]));
    return;
  }
  if (Shizu_Value_isInteger64(&argumentValues[0])) {
    *returnValue = argumentValues[0];
    return;
  }
  if (Shizu_Value_isFloat32(&argumentValues[0])) {
    float x = Shizu_Value_getFloat32(&argumentValues[0]);
    if (x != floorf(x) || x < Shizu_Integer64_Minimum || x > Shizu_Integer64_Maximum) {
      Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
      Shizu_State2_jump(state);
    } else {
      Shizu_Value_setInteger64(returnValue, (Shizu_Integer64)x);
    }
  } else if (Shizu_Runtime_Extensions_isString(state, argumentValues[0])) {
    Shizu_BigInteger10* bigInteger = Shizu_BigInteger10_createFromString(state, (Shizu_String*)Shizu_Value_getObject(&argumentValues[0]));
    Shizu_Value_setInteger64(returnValue, Shizu_BigInteger10_toInteger64(state, bigInteger));
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
}

#endif
