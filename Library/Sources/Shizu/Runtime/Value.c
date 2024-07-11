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
#include "Shizu/Runtime/Value.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Type.h"

// The DL a type is created by must not be unloaded as long as the type exists.
// For a type T defined in a DL we store in T.dl a reference to the DL in the type object.
// If T is defined in the executable we store in T.dl the null reference.
#define Shizu_definePrimitiveType(Name) \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ); \
  \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ) \
  {/*Intentionally empty.*/} \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State2* state \
    ) \
  { \
    Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), #Name, sizeof(#Name) - 1); \
    if (!type) { \
      Shizu_Dl* dl = Shizu_State1_getDlByAdr(Shizu_State2_getState1(state), &Name##_getType); \
      type = Shizu_Types_createPrimitiveType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), #Name, sizeof(#Name) - 1, dl, &Name##_typeDestroyed, &Name##_Type); \
      if (dl) { \
        Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl); \
      } \
    } \
    return type; \
  }

static Shizu_PrimitiveTypeDescriptor const Shizu_Boolean_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Boolean),
};

Shizu_definePrimitiveType(Shizu_Boolean);

static Shizu_PrimitiveTypeDescriptor const Shizu_CxxFunction_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_CxxFunction*),
};

Shizu_definePrimitiveType(Shizu_CxxFunction);

static Shizu_PrimitiveTypeDescriptor const Shizu_Float32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Float32),
};

Shizu_definePrimitiveType(Shizu_Float32);

#if 1 == Shizu_Configuration_WithFloat64

static Shizu_PrimitiveTypeDescriptor const Shizu_Float64_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Float64),
};

Shizu_definePrimitiveType(Shizu_Float64);

#endif

static Shizu_PrimitiveTypeDescriptor const Shizu_Integer32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Integer32),
};

Shizu_definePrimitiveType(Shizu_Integer32);

#if 1 == Shizu_Configuration_WithInteger64

static Shizu_PrimitiveTypeDescriptor const Shizu_Integer64_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Integer64),
};

Shizu_definePrimitiveType(Shizu_Integer64);

#endif

static Shizu_PrimitiveTypeDescriptor const Shizu_Type_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Type*),
};

Shizu_definePrimitiveType(Shizu_Type);

static Shizu_PrimitiveTypeDescriptor const Shizu_Void_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Void),
};

Shizu_definePrimitiveType(Shizu_Void);



#undef Shizu_definePrimitiveType

Shizu_Boolean
Shizu_Value_getBoolean
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isBoolean(self));
  return self->booleanValue;
}

bool
Shizu_Value_isBoolean
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Boolean == self->tag;
}

void
Shizu_Value_setBoolean
  (
    Shizu_Value* self,
    Shizu_Boolean booleanValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->booleanValue = booleanValue;
  self->tag = Shizu_Value_Tag_Boolean;
}

Shizu_CxxFunction*
Shizu_Value_getCxxFunction
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isCxxFunction(self));
  return self->cxxFunctionValue;
}

bool
Shizu_Value_isCxxFunction
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_CxxFunction == self->tag;
}

void
Shizu_Value_setCxxFunction
  (
    Shizu_Value* self,
    Shizu_CxxFunction* cxxFunctionValue
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != cxxFunctionValue);
  self->cxxFunctionValue = cxxFunctionValue;
  self->tag = Shizu_Value_Tag_CxxFunction;
}

Shizu_Float32
Shizu_Value_getFloat32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isFloat32(self));
  return self->float32Value;
}

bool
Shizu_Value_isFloat32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Float32 == self->tag;
}

void
Shizu_Value_setFloat32
  (
    Shizu_Value* self,
    Shizu_Float32 float32Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->float32Value = float32Value;
  self->tag = Shizu_Value_Tag_Float32;
}

#if 1 == Shizu_Configuration_WithFloat64

Shizu_Float64
Shizu_Value_getFloat64
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isFloat64(self));
  return self->float64Value;
}

bool
Shizu_Value_isFloat64
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Float64 == self->tag;
}

void
Shizu_Value_setFloat64
  (
    Shizu_Value* self,
    Shizu_Float64 float64Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->float64Value = float64Value;
  self->tag = Shizu_Value_Tag_Float64;
}

#endif

Shizu_Integer32
Shizu_Value_getInteger32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isInteger32(self));
  return self->integer32Value;
}

bool
Shizu_Value_isInteger32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Integer32 == self->tag;
}

void
Shizu_Value_setInteger32
  (
    Shizu_Value* self,
    Shizu_Integer32 integer32Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->integer32Value = integer32Value;
  self->tag = Shizu_Value_Tag_Integer32;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer64
Shizu_Value_getInteger64
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isInteger64(self));
  return self->integer64Value;
}

bool
Shizu_Value_isInteger64
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Integer64 == self->tag;
}

void
Shizu_Value_setInteger64
  (
    Shizu_Value* self,
    Shizu_Integer32 integer64Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->integer32Value = integer64Value;
  self->tag = Shizu_Value_Tag_Integer64;
}

#endif

Shizu_Object*
Shizu_Value_getObject
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isObject(self));
  return self->objectValue;
}

bool
Shizu_Value_isObject
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Object == self->tag;
}

void
Shizu_Value_setObject
  (
    Shizu_Value* self,
    Shizu_Object* objectValue
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != objectValue);
  self->objectValue = objectValue;
  self->tag = Shizu_Value_Tag_Object;
}

Shizu_Type*
Shizu_Value_getType
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isType(self));
  return self->typeValue;
}

bool
Shizu_Value_isType
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Type == self->tag;
}

void
Shizu_Value_setType
  (
    Shizu_Value* self,
    Shizu_Type* typeValue
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != typeValue);
  self->typeValue = typeValue;
  self->tag = Shizu_Value_Tag_Type;
}

Shizu_Void
Shizu_Value_getVoid
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isVoid(self));
  return self->voidValue;
}

bool
Shizu_Value_isVoid
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Void == self->tag;
}

void
Shizu_Value_setVoid
  (
    Shizu_Value* self,
    Shizu_Void voidValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->voidValue = voidValue;
  self->tag = Shizu_Value_Tag_Void;
}
