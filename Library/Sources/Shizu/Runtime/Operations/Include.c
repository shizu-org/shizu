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
#include "Shizu/Runtime/Gc.h"
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
      Shizu_Value_setType(returnValue, Shizu_Object_getObjectType(state, Shizu_Value_getObject(argumentValue)));
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

void
Shizu_Operations_create
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (numberOfArgumentValues < 1) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isType(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* type = Shizu_Value_getType(argumentValues + 0);
  if (!Shizu_Type_isObjectType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), type)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ObjectTypeDescriptor const* descriptor = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state),
                                                                                    Shizu_State2_getTypes(state),
                                                                                    type);
  Shizu_debugAssert(NULL != descriptor);
  Shizu_debugAssert(NULL != descriptor->construct);
  Shizu_Object* self = (Shizu_Object*)Shizu_Gc_allocateObject(state, descriptor->size);
  Shizu_Value returnValue_ = Shizu_Value_Initializer();
  Shizu_Value* argumentValues_ = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Shizu_Value) * numberOfArgumentValues);
  if (!argumentValues_) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(argumentValues_ + 0, self);
  for (Shizu_Integer32 i = 1, n = numberOfArgumentValues; i < n; ++i) {
    argumentValues_[i] = argumentValues[i];
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    descriptor->construct(state, &returnValue_, numberOfArgumentValues, argumentValues_);
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_deallocate(Shizu_State2_getState1(state), argumentValues_);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_deallocate(Shizu_State2_getState1(state), argumentValues_);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, self);
}
