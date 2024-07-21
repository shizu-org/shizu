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

static void
Shizu_BigInteger_finalize
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

static void
Shizu_BigInteger_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Shizu_BigInteger_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_BigInteger),
  .construct = &Shizu_BigInteger_constructImpl,
  .visit = (Shizu_OnVisitCallback*)NULL,
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_BigInteger_finalize,
  .dispatchSize = sizeof(Shizu_BigInteger_Dispatch),
  .dispatchInitialize = NULL,
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
  SELF->p = Shizu_State1_allocate(Shizu_State2_getState1(state), other->n);
  if (!SELF->p) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = SELF->n; i < n; ++i) {
    SELF->p[i] = other->p[i];
  }
}

static void
Shizu_BigInteger_constructFromString
  (
    Shizu_State2* state,
    Shizu_BigInteger* SELF,
    Shizu_String* other
  )
{
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
        Shizu_BigInteger_constructFromString(state, SELF, (Shizu_String*)object);
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

Shizu_Boolean
Shizu_BigInteger_isEqualTo
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_BigInteger* other
  )
{
  if (self == other) {
    return Shizu_Boolean_True;
  }
  if (self->n != other->n) {
    return Shizu_Boolean_False;
  }
  for (size_t i = 0, n = self->n; i < n; ++i) {
    if (self->p[i] != other->p[i]) {
      return Shizu_Boolean_False;
    }
  }
  return Shizu_Boolean_True;
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
