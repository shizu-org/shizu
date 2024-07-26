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
#include "Shizu/Runtime/Objects/BigFloat.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Extensions.h"
#include "Shizu/Runtime/Gc.h"

#include "Shizu/Runtime/Objects/BigInteger10.h"
#include "Shizu/Runtime/Objects/ByteArray.h"
#include "Shizu/Runtime/Objects/String.h"

static void
Shizu_BigFloat_visit
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  );

static void
Shizu_BigFloat_finalize
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  );

static void
Shizu_BigFloat_initializeDispatch
(
  Shizu_State1* state,
  Shizu_BigFloat_Dispatch* self
);

static void
Shizu_BigFloat_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_Integer32
Shizu_BigFloat_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  );

static Shizu_Boolean
Shizu_BigFloat_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigFloat* self,
    Shizu_Value const* other
  );

static Shizu_ObjectTypeDescriptor const Shizu_BigFloat_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = (Shizu_VisitTypeCallback*)NULL,
  .size = sizeof(Shizu_BigFloat),
  .construct = &Shizu_BigFloat_constructImpl,
  .visit = (Shizu_OnVisitCallback*)&Shizu_BigFloat_visit,
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_BigFloat_finalize,
  .dispatchSize = sizeof(Shizu_BigFloat_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Shizu_BigFloat_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Shizu.BigFloat", Shizu_BigFloat, Shizu_Object);

static void
Shizu_BigFloat_visit
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  )
{
  if (self->integer) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->integer);
  }
}

static void
Shizu_BigFloat_finalize
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  )
{ }

static void
Shizu_BigFloat_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_BigFloat_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->getHashValue = (Shizu_Integer32(*)(Shizu_State2*, Shizu_Object*)) & Shizu_BigFloat_getHashValueImpl;
  ((Shizu_Object_Dispatch*)self)->isEqualTo = (Shizu_Boolean(*)(Shizu_State2*, Shizu_Object*, Shizu_Value const*)) & Shizu_BigFloat_isEqualToImpl;
}

static void
Shizu_BigFloat_constructImpl
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
  if (!Shizu_Value_isObject(&argumentValues[0])) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  if (1 == numberOfArgumentValues) {
    SELF->integer = Shizu_BigInteger10_zero(state);
    SELF->scale = 0;
  } else if (3 == numberOfArgumentValues) {
    if (!Shizu_Value_isObject(&argumentValues[1]) || !Shizu_Value_isInteger32(&argumentValues[2])) {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
      Shizu_State2_jump(state);
    }
    Shizu_Object* object = Shizu_Value_getObject(&argumentValues[1]);
    if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), object->type, Shizu_BigInteger10_getType(state))) {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
      Shizu_State2_jump(state);
    }                                          
    SELF->integer = (Shizu_BigInteger10*)object;
    SELF->scale = Shizu_Value_getInteger32(&argumentValues[2]);
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  SELF->integer = Shizu_BigInteger10_create(state);
  SELF->scale = 0;
  ((Shizu_Object*)SELF)->type = TYPE;
}

static Shizu_Integer32
Shizu_BigFloat_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_BigFloat* self
  )
{ return Shizu_Object_getHashValue(state, (Shizu_Object*)self) + self->scale; }

static Shizu_Boolean
Shizu_BigFloat_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_BigFloat* self,
    Shizu_Value const* other
  )
{ return Shizu_Boolean_False; }

Shizu_BigFloat*
Shizu_BigFloat_createFromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v
  )
{
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(Shizu_BigInteger10_createFromInteger32(state, v)),
                                   Shizu_Value_InitializerInteger32(0) };
  DESCRIPTOR->construct(state, &returnValue, 3, &(argumentValues[0]));
  return SELF;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_BigFloat*
Shizu_BigFloat_createFromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v
  )
{
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(Shizu_BigInteger10_createFromInteger64(state, v)),
                                   Shizu_Value_InitializerInteger32(0) };
  DESCRIPTOR->construct(state, &returnValue, 3, &(argumentValues[0]));
  return SELF;
}

#endif

Shizu_BigFloat*
Shizu_BigFloat_zero
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(Shizu_BigInteger10_zero(state)),
                                   Shizu_Value_InitializerInteger32(0) };
  DESCRIPTOR->construct(state, &returnValue, 3, &(argumentValues[0]));
  return SELF;
}

Shizu_BigFloat*
Shizu_BigFloat_one
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(Shizu_BigInteger10_one(state)),
                                   Shizu_Value_InitializerInteger32(0) };
  DESCRIPTOR->construct(state, &returnValue, 3, &(argumentValues[0]));
  return SELF;
}

Shizu_BigFloat*
Shizu_BigFloat_createExplicit
  (
    Shizu_State2* state,
    Shizu_BigInteger10* x,
    Shizu_Integer32 s
  ) 
{
  Shizu_Type* TYPE = Shizu_BigFloat_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigFloat* SELF = (Shizu_BigFloat*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF),
                                   Shizu_Value_InitializerObject(x),
                                   Shizu_Value_InitializerInteger32(s), };
  DESCRIPTOR->construct(state, &returnValue, 3, &(argumentValues[0]));
  return SELF;
}
