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
#include "Shizu/Runtime/Objects/Biginteger.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Extensions.h"
#include "Shizu/Runtime/Gc.h"

#include "Shizu/Runtime/Objects/ByteArray.h"
#include "Shizu/Runtime/Objects/String.h"

#include "Shizu/Runtime/Objects/BigInteger/fromInteger32.h"
#include "Shizu/Runtime/Objects/BigInteger/fromInteger64.h"
#include "Shizu/Runtime/Objects/BigInteger/fromString.h"
#include "Shizu/Runtime/Objects/BigInteger/compareMagnitudes.h"

static void
Shizu_BigInteger_finalize
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

static void
Shizu_BigInteger_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_BigInteger_Dispatch* self
  );

static void
Shizu_BigInteger_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_Integer32
Shizu_BigInteger_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

static Shizu_Boolean
Shizu_BigInteger_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_Value const* other
  );

static Shizu_ObjectTypeDescriptor const Shizu_BigInteger_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = (Shizu_VisitTypeCallback*)NULL,
  .size = sizeof(Shizu_BigInteger),
  .construct = &Shizu_BigInteger_constructImpl,
  .visit = (Shizu_OnVisitCallback*)NULL,
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_BigInteger_finalize,
  .dispatchSize = sizeof(Shizu_BigInteger_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Shizu_BigInteger_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Shizu.BigInteger", Shizu_BigInteger, Shizu_Object);

static void
Shizu_BigInteger_finalize
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  )
{
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self->p);
  self->p = NULL;
  self->n = 0;
}

static void
Shizu_BigInteger_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_BigInteger_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->getHashValue = (Shizu_Integer32(*)(Shizu_State2*, Shizu_Object*)) & Shizu_BigInteger_getHashValueImpl;
  ((Shizu_Object_Dispatch*)self)->isEqualTo = (Shizu_Boolean(*)(Shizu_State2*, Shizu_Object*, Shizu_Value const*)) & Shizu_BigInteger_isEqualToImpl;
}

Shizu_Integer32
Shizu_BigInteger_compare
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_BigInteger* other
  )
{
  if (self->sign == other->sign) {
    return compareMagnitudes(state, self->p, self->n, other->p, other->n); 
  } else {
    // We have different signs: If self->sign = true then other->sign = false, that is, "other" is negative and "self" is non-negative.
    // Hence "self" is greater than other "other" and we return +1. Otherwise "self" is less than "other" and we return -1.
    return self->sign ? +1 : -1;
  }
}

Shizu_Integer32
Shizu_BigInteger_compareInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_Integer32 other
  )
{
  bool sign = other >= INT32_C(0);
  // Fast path: If the signs are different, then the non-negative one is greater.
  if (self->sign != sign) {
    return self->sign ? +1 : -1;
  }
  // Slow path: The signs are equal.
  return Shizu_BigInteger_compare(state, self, Shizu_BigInteger_createFromInteger32(state, other));

}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer32
Shizu_BigInteger_compareInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_Integer64 other
  )
{
  bool sign = other >= INT64_C(0);
  // Fast path: If the signs are different, then the non-negative one is greater.
  if (self->sign != sign) {
    return self->sign ? +1 : -1;
  }
  // Slow path: The signs are equal.
  return Shizu_BigInteger_compare(state, self, Shizu_BigInteger_createFromInteger64(state, other));
}

#endif

Shizu_Boolean
Shizu_BigInteger_isZero
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  )
{
  return !self->p[0];
}

Shizu_BigInteger*
Shizu_BigInteger_negate
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  )
{
  if (Shizu_BigInteger_isZero(state, self)) {
    return self;
  }
  Shizu_BigInteger* clone = Shizu_BigInteger_createFromBigInteger(state, self);
  clone->sign = !clone->sign;
  return clone;
}

static void
Shizu_BigInteger_constructFromBigInteger
  (
    Shizu_State2* state,
    Shizu_BigInteger* SELF,
    Shizu_BigInteger* other
  )
{
  SELF->n = other->n;
  SELF->sign = other->sign;
  SELF->p = Shizu_State1_allocate(Shizu_State2_getState1(state), other->n * sizeof(uint8_t));
  if (!SELF->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = SELF->n; i < n; ++i) {
    SELF->p[i] = other->p[i];
  }
}

static void
Shizu_BigInteger_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_BigInteger* SELF = (Shizu_BigInteger*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  if (numberOfArgumentValues == 1) {
    SELF->p = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint8_t));
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
      if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), object->type, Shizu_BigInteger_getType(state))) {
        Shizu_BigInteger_constructFromBigInteger(state, SELF, (Shizu_BigInteger*)object);
      } else if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), object->type, Shizu_String_getType(state))) {
        Shizu_BigInteger_fromString(state, (Shizu_String*)object, &SELF->sign, &SELF->p, &SELF->n);
      } else {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
        Shizu_State2_jump(state);
      }
    } else if (Shizu_Value_isInteger32(&argumentValues[1])) {
      Shizu_BigInteger_fromInteger32(state, Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]), &SELF->sign, &SELF->p, &SELF->n);
    #if 1 == Shizu_Configuration_WithInteger64
    } else if (Shizu_Value_isInteger64(&argumentValues[1])) {
      Shizu_BigInteger_fromInteger64(state, Shizu_Runtime_Extensions_getInteger64Value(state, &argumentValues[1]), &SELF->sign, &SELF->p, &SELF->n);
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
Shizu_BigInteger_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  )
{
  return (self->sign ? 1231 : 1237) + (self->p[0] * self->n);
}

static Shizu_Boolean
Shizu_BigInteger_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
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
  if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Object_getObjectType(state, otherObject), Shizu_BigInteger_getType(state))) {
    Shizu_BigInteger* x = self;
    Shizu_BigInteger* y = (Shizu_BigInteger*)otherObject;
    return x->sign == y->sign && !compareMagnitudes(state, x->p, x->n, y->p, y->n);
  } else {
    return Shizu_Boolean_False;
  }
}

Shizu_BigInteger*
Shizu_BigInteger_create
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

Shizu_BigInteger*
Shizu_BigInteger_createFromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* SELF = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerInteger32(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_BigInteger*
Shizu_BigInteger_createFromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v
  )
{
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* SELF = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerInteger64(v), };
  DESCRIPTOR->construct(state, &returnValue, 2, &(argumentValues[0]));
  return SELF;
}

#endif

Shizu_BigInteger*
Shizu_BigInteger_createFromString
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

Shizu_BigInteger*
Shizu_BigInteger_createFromBigInteger
  (
    Shizu_State2* state,
    Shizu_BigInteger* v
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

Shizu_String*
Shizu_BigInteger_toString
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
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

// if |x| >= |y| compute |x| - |y| and return true.
// otherwise compute |y| - |x| and return false. 
static bool
subtractDigits
  (
    Shizu_State2* state,
    uint8_t const* x,
    Shizu_Integer32 xn,
    uint8_t const* y,
    Shizu_Integer32 yn,
    uint8_t** pz,
    Shizu_Integer32* pzn
  )
{
  Shizu_Integer32 compareResult = compareMagnitudes(state, x, xn, y, yn);
  if (compareResult < 0) {
    uint8_t const* t = x;
    x = y;
    y = t;
    Shizu_Integer32 tn = xn;
    xn = yn;
    yn = tn;
  }
  Shizu_Integer32 zn = xn;
  uint8_t* z = Shizu_State1_allocate(Shizu_State2_getState1(state), zn);
  if (!z) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  bool borrow = false;
  // Subtract common part.
  while (yn > 0) {
    uint8_t u = x[--xn];
    uint8_t v = y[--yn] + (borrow ? 1 : 0);
    uint8_t r = (u < v) ? v - u : u - v;
    bool borrow = (u < v);
    z[xn] = r;
  }
  // Subtract part specific to the longer number while propagating borrow.
  while (xn > 0 && borrow) { 
    uint8_t u = x[--xn];
    uint8_t v = (borrow ? 1 : 0);
    uint8_t r = (u < v) ? v - u : u - v;
    bool borrow = (u < v);
    z[xn] = r;
  }
  // Subtract part specific to the longer number.
  while (xn > 0)  {
    z[--xn] = x[xn];
  }
  *pz = z;
  *pzn = zn;
  return compareResult >= 0;
}

static void
addDigits
  (
    Shizu_State2* state,
    uint8_t const* x,
    Shizu_Integer32 xn,
    uint8_t const* y,
    Shizu_Integer32 yn,
    uint8_t** pz,
    Shizu_Integer32* pzn
)
{
  if (xn < yn) {
    uint8_t const* t1 = x;
    x = y;
    y = t1;
    Shizu_Integer32 t2 = xn;
    xn = yn;
    yn = t2;
  }
  Shizu_Integer32 zn = xn;
  uint8_t* z = Shizu_State1_allocate(Shizu_State2_getState1(state), xn);
  if (!z) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  bool carry = false;
  // Add common part.
  while (yn > 0) {
    uint8_t r = x[--xn] + y[--yn] + (carry ? 1 : 0);
    z[xn] = r % 10;
    carry = r / 10;
  }
  // Add part specific to the longer number while propagating carry.
  while (xn > 0 && carry) {
    carry = (0 == (z[--xn] = x[xn] + 1));
  }
  // Add part specific to the longer number.
  while (xn > 0) {
    z[--xn] = x[xn];
  }
  // Handle remaining carry (if any).
  if (carry) {
    uint8_t* w = Shizu_State1_allocate(Shizu_State2_getState1(state), zn + 1);
    if (!w) {
      Shizu_State1_deallocate(Shizu_State2_getState1(state), z);
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    memcpy(w + 1, z, zn);
    w[0] = carry;
    zn++;
    Shizu_State1_deallocate(Shizu_State2_getState1(state), z);
    z = w;
  }
  *pz = z;
  *pzn = zn;
}

Shizu_Integer32
Shizu_BigInteger_toInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
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
Shizu_BigInteger_toInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
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

Shizu_BigInteger*
Shizu_BigInteger_add
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_BigInteger* other
  )
{
  if (Shizu_BigInteger_isZero(state, self)) {
    return other;
  }
  if (Shizu_BigInteger_isZero(state, other)) {
    return self;
  }
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* RESULT = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  // Neither "self" nor "other" is zero. If their signs are equal, then this reduces to an addition of the magnitudes of "self" and "other"
  // and the sign of the result is the sign of "self". Otherwise we invoke "subtractDigits". If "subtractDigits" returns "true" then "self"
  // >= "other" and hence the sign is the sign of "self". If "subtractDigits" returns "false" then "self" < "other" and the sign is the
  // sign of "other". "subtractDigits" may return a number of leading zeroes, we also fix that.
  if (self->sign == other->sign) {
    // Signs are the same. Add magnitudes. Retain sign.
    RESULT->sign = self->sign;
    addDigits(state, self->p, self->n, other->p, other->n, &RESULT->p, &RESULT->n);
  } else {
    // Signs are different.
    bool result = subtractDigits(state, self->p, self->n, other->p, other->n, &RESULT->p, &RESULT->n);
    RESULT->sign = result ? self->sign : other->sign;
    // Remove leading zeroes and adjust sign.
    size_t count = 0;
    for (size_t i = 0, n = RESULT->n; i < n; ++i) {
      if (RESULT->p[0]) {
        break;
      } else {
        count++;
      }
    }
    if (count) {
      // Do not remove the only remaining zero.
      if (count == RESULT->n) {
        count--;
      }
      memmove(RESULT->p, RESULT->p + count, RESULT->n - count);
      RESULT->n -= count;
    }
    if (RESULT->n == 1 && !RESULT->p[0]) {
      RESULT->sign = Shizu_Boolean_True;
    }
  }
  //
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

Shizu_BigInteger*
Shizu_BigInteger_subtract
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_BigInteger* other
  )
{
  if (Shizu_BigInteger_isZero(state, self)) {
    return Shizu_BigInteger_negate(state, other);
  }
  if (Shizu_BigInteger_isZero(state, other)) {
    return self;
  }
  Shizu_Type* TYPE = Shizu_BigInteger_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger* RESULT = (Shizu_BigInteger*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  // Neither "self" nor "other" is zero. If their signs are different, then this reduces to an addition of the magnitudes of "self" and "other"
  // and the sign of the result is the sign of "self". Otherwise we invoke "subtractDigits". If "subtractDigits" returns "true" then "self"
  // >= "other" and hence the sign is the sign of "self". If "subtractDigits" returns "false" then "self" < "other" and the sign is the
  // sign of minus "self". "subtractDigits" may return a number of leading zeroes, we also fix that.
  if (self->sign != other->sign) {
    RESULT->sign = self->sign;
    addDigits(state, self->p, self->n, other->p, other->n, &RESULT->p, &RESULT->n);
  } else {
    bool result = subtractDigits(state, self->p, self->n, other->p, other->n, &RESULT->p, &RESULT->n);
    RESULT->sign = result ? self->sign : !self->sign;
    // Remove leading zeroes and adjust sign.
    size_t count = 0;
    for (size_t i = 0, n = RESULT->n; i < n; ++i) {
      if (RESULT->p[0]) {
        break;
      } else {
        count++;
      }
    }    
    if (count) {
      // Do not remove the only remaining zero.
      if (count == RESULT->n) {
        count--;
      }
      memmove(RESULT->p, RESULT->p + count, RESULT->n - count);
      RESULT->n -= count;
    }
    if (RESULT->n == 1 && !RESULT->p[0]) {
      RESULT->sign = Shizu_Boolean_True;;
    }
  }
  //
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}
