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
#include "Shizu/Runtime/Operations/Include.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Object.h"
#include "Shizu/Runtime/CxxUtilities.h"

void
Shizu_Operations_typeOf
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_debugAssert(NULL != returnValue);
  Shizu_debugAssert(NULL != argumentValues);
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value* argumentValue = &(argumentValues[0]);
  switch (argumentValue->tag) {
    case Shizu_Value_Tag_Boolean: {
      Shizu_Value_setType(returnValue, Shizu_Boolean_getType(state));
    } break;
    case Shizu_Value_Tag_CxxFunction: {
      Shizu_Value_setType(returnValue, Shizu_CxxFunction_getType(state));
    } break;
    case Shizu_Value_Tag_Float32: {
      Shizu_Value_setType(returnValue, Shizu_Float32_getType(state));
    } break;
    case Shizu_Value_Tag_Integer32: {
      Shizu_Value_setType(returnValue, Shizu_Integer32_getType(state));
    } break;
    case Shizu_Value_Tag_Object: {
      Shizu_Value_setType(returnValue, Shizu_State2_getObjectType(state, Shizu_Value_getObject(argumentValue)));
    } break;
    case Shizu_Value_Tag_Type: {
      Shizu_Value_setType(returnValue, Shizu_Type_getType(state));
    } break;
    case Shizu_Value_Tag_Void: {
      Shizu_Value_setType(returnValue, Shizu_Void_getType(state));
    } break;
    default: {
      Shizu_unreachableCodeReached(__FILE__, __LINE__);
    } break;
  };
}
