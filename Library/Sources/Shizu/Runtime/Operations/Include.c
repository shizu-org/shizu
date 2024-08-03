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
#include "Shizu/Runtime/Operations/Include.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Object.h"
#include "Shizu/Runtime/Objects/String.h"
#include "Shizu/Runtime/Gc.h"
#include "Shizu/Runtime/Extensions.h"

#include "Shizu/Runtime/Operations/ToInteger.h"

#include "Shizu/Runtime/Operations/ToFloat32.h"
#include "Shizu/Runtime/Include.h"

#if defined(Shizu_Configuration_WithTests)

#include "Shizu/Runtime/Operations/Utilities/BigInt/Include.h"

void
Shizu_Operations_toFloat_tests
  (
    Shizu_State2* state
  )
{
  bigint_tests(Shizu_State2_getState1(state));
  Shizu_Operations_toFloat32Version1_tests(state);
}

#endif

void
Shizu_Operations_toFloat32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{ Shizu_Operations_toFloat32Version1(state, returnValue, numberOfArgumentValues, argumentValues); }

void
Shizu_Operations_toInteger32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{ Shizu_Operations_toInteger32Version1(state, returnValue, numberOfArgumentValues, argumentValues); }

#if 1 == Shizu_Configuration_WithInteger64

void
Shizu_Operations_toInteger64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{ Shizu_Operations_toInteger64Version1(state, returnValue, numberOfArgumentValues, argumentValues); }

#endif

void
Shizu_Operations_getType
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Cxx_Debug_assert(NULL != returnValue);
  Shizu_Cxx_Debug_assert(NULL != argumentValues);
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* name = Shizu_Runtime_Extensions_getStringValue(state, &argumentValues[0]);
  Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_String_getBytes(state, name), Shizu_String_getNumberOfBytes(state, name));
  if (!type) {
    Shizu_State2_setStatus(state, Shizu_Status_TypeNotFound);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setType(returnValue, type);
}

void
Shizu_Operations_typeOf
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Cxx_Debug_assert(NULL != returnValue);
  Shizu_Cxx_Debug_assert(NULL != argumentValues);
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
      Shizu_Cxx_unreachableCodeReached();
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
  if (!Shizu_Types_isObjectType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), type)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ObjectTypeDescriptor const* descriptor = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state),
                                                                                    Shizu_State2_getTypes(state),
                                                                                    type);
  Shizu_Cxx_Debug_assert(NULL != descriptor);
  Shizu_Cxx_Debug_assert(NULL != descriptor->construct);
  Shizu_Object* self = (Shizu_Object*)Shizu_Gc_allocateObject(state, descriptor->size);
  Shizu_Value returnValue_ = Shizu_Value_InitializerVoid(Shizu_Void_Void);
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

void
Shizu_Operations_not
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (numberOfArgumentValues != 1) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isBoolean(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setBoolean(returnValue, Shizu_Value_getBoolean(argumentValues + 0));
}

void
Shizu_Operations_and
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (numberOfArgumentValues != 2) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isBoolean(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isBoolean(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setBoolean(returnValue, Shizu_Value_getBoolean(argumentValues + 0) &&
                                      Shizu_Value_getBoolean(argumentValues + 1));
}

void
Shizu_Operations_or
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isBoolean(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isBoolean(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setBoolean(returnValue, Shizu_Value_getBoolean(argumentValues + 0) ||
                                      Shizu_Value_getBoolean(argumentValues + 1));
}

// MSVC does not offer proper compiler intrinsics AND its inline assembler only supports x86.
// There are two choices:
// a) Use MASM to support the implementation of Shizu_Operations_(add|subtract|multiply|divide)_i32.
//    This would, in general, make use of a few assembler instructions like
//    "ADD" (https://www.felixcloutier.com/x86/add), "SUB" (https://www.felixcloutier.com/x86/sub),
//    "IMUL" (https://www.felixcloutier.com/x86/imul), "IDIV" (https://www.felixcloutier.com/x86/idiv)
// b) Promote int32_t to int64_t before performing the operation.
//    Use the lower 32 bits of the int64_t result.
// We have selected option b).
void
Shizu_Operations_add_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 x = Shizu_Value_getInteger32(argumentValues + 0);
  Shizu_Integer32 y = Shizu_Value_getInteger32(argumentValues + 1);
  Shizu_Integer32 z;
#if Shizu_Configuration_CompilerC_Gcc == Shizu_Configuration_CompilerC || \
    Shizu_Configuration_CompilerC_Clang == Shizu_Configuration_CompilerC
  __builtin_add_overflow(x, y, &z);
#elif Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
  int64_t w = ((int64_t)x) + ((int64_t)y);
  z = (int32_t)(w & 0xffffffff);
#else
  #error("compiler not yet supported")
#endif
  Shizu_Value_setInteger32(returnValue, z);
}

void
Shizu_Operations_add_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float32 x = Shizu_Value_getFloat32(argumentValues + 0);
  Shizu_Float32 y = Shizu_Value_getFloat32(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x + y);
}

#if 1 == Shizu_Configuration_WithFloat64

void
Shizu_Operations_add_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float64 x = Shizu_Value_getFloat64(argumentValues + 0);
  Shizu_Float64 y = Shizu_Value_getFloat64(argumentValues + 1);
  Shizu_Value_setFloat64(returnValue, x + y);
}

#endif

void
Shizu_Operations_subtract_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 x = Shizu_Value_getInteger32(argumentValues + 0);
  Shizu_Integer32 y = Shizu_Value_getInteger32(argumentValues + 1);
  Shizu_Integer32 z;
#if Shizu_Configuration_CompilerC_Gcc == Shizu_Configuration_CompilerC || \
    Shizu_Configuration_CompilerC_Clang == Shizu_Configuration_CompilerC
  __builtin_sub_overflow(x, y, &z);
#elif Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
  int64_t w = ((int64_t)x) - ((int64_t)y);
  z = (int32_t)(w & 0xffffffff);
#else
  #error("compiler not yet supported")
#endif
  Shizu_Value_setInteger32(returnValue, z);
}

void
Shizu_Operations_subtract_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float32 x = Shizu_Value_getFloat32(argumentValues + 0);
  Shizu_Float32 y = Shizu_Value_getFloat32(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x - y);
}

#if 1 == Shizu_Configuration_WithFloat64

void
Shizu_Operations_subtract_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float64 x = Shizu_Value_getFloat64(argumentValues + 0);
  Shizu_Float64 y = Shizu_Value_getFloat64(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x - y);
}

#endif

void
Shizu_Operations_multiply_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 x = Shizu_Value_getInteger32(argumentValues + 0);
  Shizu_Integer32 y = Shizu_Value_getInteger32(argumentValues + 1);
  Shizu_Integer32 z;
#if Shizu_Configuration_CompilerC_Gcc == Shizu_Configuration_CompilerC || \
    Shizu_Configuration_CompilerC_Clang == Shizu_Configuration_CompilerC
  __builtin_mul_overflow(x, y, &z);
#elif Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
  int64_t w = ((int64_t)x) * ((int64_t)y);
  z = (int32_t)(w & 0xffffffff);
#else
  #error("compiler not yet supported")
#endif
  Shizu_Value_setInteger32(returnValue, z);
}

void
Shizu_Operations_multiply_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float32 x = Shizu_Value_getFloat32(argumentValues + 0);
  Shizu_Float32 y = Shizu_Value_getFloat32(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x * y);
}

#if 1 == Shizu_Configuration_WithFloat64

void
Shizu_Operations_multiply_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float64 x = Shizu_Value_getFloat64(argumentValues + 0);
  Shizu_Float64 y = Shizu_Value_getFloat64(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x * y);
}

#endif

void
Shizu_Operations_divide_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isInteger32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 x = Shizu_Value_getInteger32(argumentValues + 0);
  Shizu_Integer32 y = Shizu_Value_getInteger32(argumentValues + 1);
  if (!y) {
    Shizu_State2_setStatus(state, Shizu_Status_DivisionByZero);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setInteger32(returnValue, x / y);
}

void
Shizu_Operations_divide_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat32(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float32 x = Shizu_Value_getFloat32(argumentValues + 0);
  Shizu_Float32 y = Shizu_Value_getFloat32(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x / y);
}

#if 1 == Shizu_Configuration_WithFloat64

void
Shizu_Operations_divide_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 0)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isFloat64(argumentValues + 1)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Float64 x = Shizu_Value_getFloat64(argumentValues + 0);
  Shizu_Float64 y = Shizu_Value_getFloat64(argumentValues + 1);
  Shizu_Value_setFloat32(returnValue, x / y);
}

#endif
