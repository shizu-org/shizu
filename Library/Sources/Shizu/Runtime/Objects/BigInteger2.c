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
#include "Shizu/Runtime/Objects/BigInteger2.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Extensions.h"
#include "Shizu/Runtime/Gc.h"

#include "Shizu/Runtime/Objects/ByteArray.h"
#include "Shizu/Runtime/Objects/String.h"

#include "Shizu/Runtime/Objects/BigInteger2/fromInteger32.h"
#include "Shizu/Runtime/Objects/BigInteger2/fromInteger64.h"
#include "Shizu/Runtime/Objects/BigInteger2/addSubtract.h"
#include "Shizu/Runtime/Objects/BigInteger2/compareMagnitudes.h"



static void
Shizu_BigInteger2_finalize
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

static void
Shizu_BigInteger2_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_BigInteger2_Dispatch* self
  );

static void
Shizu_BigInteger2_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_Integer32
Shizu_BigInteger2_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

static Shizu_Boolean
Shizu_BigInteger2_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Value const* other
  );

static Shizu_ObjectTypeDescriptor const Shizu_BigInteger2_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = (Shizu_VisitTypeCallback*)NULL,
  .size = sizeof(Shizu_BigInteger2),
  .construct = &Shizu_BigInteger2_constructImpl,
  .visit = (Shizu_OnVisitCallback*)NULL,
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_BigInteger2_finalize,
  .dispatchSize = sizeof(Shizu_BigInteger2_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Shizu_BigInteger2_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Shizu.BigInteger2", Shizu_BigInteger2, Shizu_Object);

static void
Shizu_BigInteger2_finalize
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self->p);
  self->p = NULL;
  self->n = 0;
}

static void
Shizu_BigInteger2_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_BigInteger2_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->getHashValue = (Shizu_Integer32(*)(Shizu_State2*, Shizu_Object*)) & Shizu_BigInteger2_getHashValueImpl;
  ((Shizu_Object_Dispatch*)self)->isEqualTo = (Shizu_Boolean(*)(Shizu_State2*, Shizu_Object*, Shizu_Value const*)) & Shizu_BigInteger2_isEqualToImpl;
}

Shizu_Integer32
Shizu_BigInteger2_compare
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  )
{
  if (self->sign == other->sign) {
    return _Shizu_BigInteger2_compareMagnitudes(state, self, other); 
  } else {
    // We have different signs: If self->sign = true then other->sign = false, that is, "other" is negative and "self" is non-negative.
    // Hence "self" is greater than other "other" and we return +1. Otherwise "self" is less than "other" and we return -1.
    return self->sign ? +1 : -1;
  }
}

Shizu_Integer32
Shizu_BigInteger2_compareInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer32 other
  )
{
  bool sign = other >= INT32_C(0);
  // Fast path: If the signs are different, then the non-negative one is greater.
  if (self->sign != sign) {
    return self->sign ? +1 : -1;
  }
  // Slow path: The signs are equal.
  return Shizu_BigInteger2_compare(state, self, Shizu_BigInteger2_createFromInteger32(state, other));

}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer32
Shizu_BigInteger2_compareInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer64 other
  )
{
  bool sign = other >= INT64_C(0);
  // Fast path: If the signs are different, then the non-negative one is greater.
  if (self->sign != sign) {
    return self->sign ? +1 : -1;
  }
  // Slow path: The signs are equal.
  return Shizu_BigInteger2_compare(state, self, Shizu_BigInteger2_createFromInteger64(state, other));
}

#endif

Shizu_Boolean
Shizu_BigInteger2_isZero
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  return !self->p[0];
}

Shizu_BigInteger2*
Shizu_BigInteger2_negate
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  if (Shizu_BigInteger2_isZero(state, self)) {
    return self;
  }
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* RESULT = (Shizu_BigInteger2*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  RESULT->n = self->n;
  RESULT->sign = !self->sign;
  RESULT->p = Shizu_State1_allocate(Shizu_State2_getState1(state), self->n * sizeof(uint8_t));
  if (!RESULT->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = self->n; i < n; ++i) {
    RESULT->p[i] = self->p[i];
  }
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

static void
Shizu_BigInteger2_constructFromBigInteger2
  (
    Shizu_State2* state,
    Shizu_BigInteger2* SELF,
    Shizu_BigInteger2* other
  )
{
  uint32_t* p = Shizu_State1_reallocate(Shizu_State2_getState1(state), SELF->p, other->n * sizeof(uint32_t));
  if (!p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  SELF->p = p;
  SELF->n = other->n;
  SELF->sign = other->sign;
  for (size_t i = 0, n = SELF->n; i < n; ++i) {
    SELF->p[i] = other->p[i];
  }
}

#if 0
static void
Shizu_BigInteger2_constructFromBigInteger10
  (
    Shizu_State2* state,
    Shizu_BigInteger2* SELF,
    Shizu_BigInteger2* other
  )
{
  SELF->n = other->n;
  SELF->sign = other->sign;
  SELF->p = Shizu_State1_allocate(Shizu_State2_getState1(state), other->n * sizeof(uint32_t));
  if (!SELF->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = SELF->n; i < n; ++i) {
    SELF->p[i] = other->p[i];
  }
}
#endif

static void
Shizu_BigInteger2_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_BigInteger2* SELF = (Shizu_BigInteger2*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  if (numberOfArgumentValues == 1) {
    SELF->p = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint32_t));
    if (!SELF->p) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    SELF->p[0] = 0;
    SELF->n = 1;
    SELF->sign = Shizu_Boolean_True;
  } else if (numberOfArgumentValues == 2) {
    if (Shizu_Value_isObject(&argumentValues[1])) {
      Shizu_Object* object = Shizu_Value_getObject(&argumentValues[1]);
      if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), object->type, Shizu_BigInteger2_getType(state))) {
        Shizu_BigInteger2_constructFromBigInteger2(state, SELF, (Shizu_BigInteger2*)object);
     #if 0
      } else if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), object->type, Shizu_String_getType(state))) {
        Shizu_BigInteger2_fromString(state, (Shizu_String*)object, &SELF->sign, &SELF->p, &SELF->n);
      #endif
      } else {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
        Shizu_State2_jump(state);
      }
    
    } else
    if (Shizu_Value_isInteger32(&argumentValues[1])) {
      _Shizu_BigInteger2_fromInteger32(state, Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]), &SELF->sign, &SELF->p, &SELF->n);
    #if 1 == Shizu_Configuration_WithInteger64
    } else if (Shizu_Value_isInteger64(&argumentValues[1])) {
      _Shizu_BigInteger2_fromInteger64(state, Shizu_Runtime_Extensions_getInteger64Value(state, &argumentValues[1]), &SELF->sign, &SELF->p, &SELF->n);
    #endif
    } else {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
      Shizu_State2_jump(state);
    }
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  ((Shizu_Object*)SELF)->type = TYPE;
}

static Shizu_Integer32
Shizu_BigInteger2_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  return (self->sign ? 1231 : 1237) + (self->p[0] * self->n);
}

static Shizu_Boolean
Shizu_BigInteger2_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Value const* other
  )
{
  if (!Shizu_Value_isObject(other)) {
    return Shizu_Boolean_False;
  }
  Shizu_Object* otherObject = Shizu_Value_getObject(other);
  if ((Shizu_Object*)self == otherObject) {
    return Shizu_Boolean_True;
  }
  if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Object_getObjectType(state, otherObject), Shizu_BigInteger2_getType(state))) {
    Shizu_BigInteger2* x = self;
    Shizu_BigInteger2* y = (Shizu_BigInteger2*)otherObject;
    return x->sign == y->sign && !_Shizu_BigInteger2_compareMagnitudes(state, x, y);
  } else {
    return Shizu_Boolean_False;
  }
}

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* SELF = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF), };
  DESCRIPTOR->construct(state, &returnValue, 1, &(argumentValues[0]));
  return SELF;
}
#endif

Shizu_BigInteger2*
Shizu_BigInteger2_createFromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* SELF = (Shizu_BigInteger2*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerInteger32(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_BigInteger2*
Shizu_BigInteger2_createFromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* SELF = (Shizu_BigInteger2*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerInteger64(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

#endif

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_createFromString
  (
    Shizu_State2* state,
    Shizu_String* v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* SELF = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

Shizu_BigInteger2*
Shizu_BigInteger2_createFromBigInteger2
  (
    Shizu_State2* state,
    Shizu_BigInteger2* v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* SELF = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

Shizu_String*
Shizu_BigInteger2_toString
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  // Set this to logically true if you want the '+' sign to be displayed.
  // Set this to logically false otherwise.
  static const Shizu_Boolean EMIT_PLUS_SIGN = Shizu_Boolean_False;
  Shizu_ByteArray* buffer = Shizu_ByteArray_create(state);
  if (!self->sign) {
    uint8_t c = '-';
    Shizu_ByteArray_appendRawBytes(state, buffer, &c, sizeof(uint8_t));
  } else if (EMIT_PLUS_SIGN) {
    uint8_t c = '+';
    Shizu_ByteArray_appendRawBytes(state, buffer, &c, sizeof(uint8_t));
  }
  for (size_t i = 0, n = self->n; i < n; ++i) {
    uint8_t c = self->p[i] + '0';
    Shizu_ByteArray_appendRawBytes(state, buffer, &c, sizeof(uint8_t));
  }
  return Shizu_String_create(state, Shizu_ByteArray_getRawBytes(state, buffer), Shizu_ByteArray_getNumberOfRawBytes(state, buffer));
}

Shizu_Integer32
Shizu_BigInteger2_toInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  // If we could store somewhere Shizu_Integer32_Minimum and Shizu_Integer32_Maximum as Shizu_BigInteger values,
  // then a test if this would overflow would be very fast.
  if (Shizu_BigInteger_compareInteger32(state, self, Shizu_Integer32_Minimum) < 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  if (Shizu_BigInteger_compareInteger32(state, self, Shizu_Integer32_Maximum) > 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 v = 0;
  for (size_t i = 0, n = self->n; i < n; ++i) {
    uint8_t digit = self->p[i];
    v *= 10;
    v += digit;
  }
  if (!self->sign) {
    v *= -1;
  }
  return v;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer64
Shizu_BigInteger2_toInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  )
{
  // If we could store somewhere Shizu_Integer75_Minimum and Shizu_Integer64_Maximum as Shizu_BigInteger values,
  // then a test if this would overflow would be very fast.
  if (Shizu_BigInteger_compareInteger64(state, self, Shizu_Integer64_Minimum) < 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  if (Shizu_BigInteger_compareInteger64(state, self, Shizu_Integer64_Maximum) > 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Integer64 v = 0;
  for (size_t i = 0, n = self->n; i < n; ++i) {
    uint8_t digit = self->p[i];
    v *= 10;
    v += digit;
  }
  if (!self->sign) {
    v *= -1;
  }
  return v;
}

#endif
#endif

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_add
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  )
{
  return add(state, self, other);
}
#endif

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_subtract
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  )
{
  return subtract(state, self, other);
}
#endif

Shizu_BigInteger2*
Shizu_BigInteger2_zero
  (
    Shizu_State2* state
  )
{
  // As the number is not zero, this is as easy as appending n zero digits.
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* RESULT = (Shizu_BigInteger2*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  RESULT->p = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint32_t));
  if (!RESULT->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  RESULT->p[0] = UINT32_C(0);
  RESULT->n = 1;
  RESULT->sign = Shizu_Boolean_True;
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

Shizu_BigInteger2*
Shizu_BigInteger2_one
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* RESULT = (Shizu_BigInteger2*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  RESULT->p = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint32_t));
  if (!RESULT->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  RESULT->p[0] = UINT32_C(1);
  RESULT->n = 1;
  RESULT->sign = Shizu_Boolean_True;
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_multiply2
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_Integer32 n
  )
{
  if (!self || n < 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  // x = 0 or n = 0 => x * 10^n = x
  if (Shizu_BigInteger2_isZero(state, self) || 0 == n) {
    return self;
  }
  // There is an upper limit for the number of digits.
  if (UINT32_MAX - self->n < n) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  // As the number is not zero, this is as easy as appending n zero digits.
  Shizu_Type* TYPE = Shizu_BigInteger2_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger2* RESULT = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  RESULT->p = Shizu_State1_allocate(Shizu_State2_getState1(state), self->n + n);
  if (!RESULT->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  memcpy(RESULT->p, self->p, self->n);
  for (size_t i = 0; i < n; ++i) {
    RESULT->p[self->n + i] = 0;
  }
  RESULT->n = self->n + n;
  RESULT->sign = self->sign;
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

Shizu_BigIntege2r*
Shizu_BigInteger2_divide2
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_Integer32 n
  )
{
  if (!self || n < 0) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  // if self->p[0] = 0 then self->n = 1 hence self = 0 and hence floor(self / 10^n) for any n>=0 is 0.
  // if self->p[0] != 0 then self->n >= 1. Hence we have a non-zero digit followed by n - 1 digits.
  // Hence if we divide by 10^n then the result is 0. Example 997 is n = 3. Dividing by 10^n = 10^3 = 10 * 10 * 10 yields 0.
  if (self->p[0] == 0 || self->n <= n) {
    return Shizu_BigInteger_zero(state);
  }
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* RESULT = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  RESULT->p = Shizu_State1_allocate(Shizu_State2_getState1(state), self->n - n);
  if (!RESULT->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  memcpy(RESULT->p, self->p, self->n - n);
  RESULT->n = self->n - n;
  RESULT->sign = self->sign;
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}
#endif
