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
#include "Shizu/Runtime/Value.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Object.h"
#include "Shizu/Runtime/Type.h"

// isnan
#include <math.h>



// @return A hash value within the bounds of [Shizu_Integer32_Minimum, Shizu_Integer32_Maximum].
static Shizu_Integer32
hashBytes
  (
    Shizu_State2* state,
    uint8_t const* p,
    size_t n
  );

static Shizu_Integer32
hashBytes
  (
    Shizu_State2* state,
    uint8_t const* p,
    size_t n
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != p);
  Shizu_Integer32 h = n;
  Shizu_Integer32 step = 1;
  for (; n >= step; n -= step) {
    h = h * 37 + (Shizu_Integer32)p[n];
  }
  return h;
}

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
    Shizu_Integer64 integer64Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->integer64Value = integer64Value;
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

Shizu_Boolean
Shizu_Value_isEqualTo
  (
    Shizu_State2* state,
    Shizu_Value const* x,
    Shizu_Value const* y
  )
{
  if (Shizu_Value_Tag_Object == x->tag) {
    return Shizu_Object_isEqualTo(state, Shizu_Value_getObject(x), y);
  }
  if (x->tag != y->tag) {
    return Shizu_Boolean_False;
  }
  switch (x->tag) {
    case Shizu_Value_Tag_Boolean: {
      return x->booleanValue == y->booleanValue;
    } break;
    case Shizu_Value_Tag_CxxFunction: {
      return x->cxxFunctionValue == y->cxxFunctionValue;
    } break;
    case Shizu_Value_Tag_Float32: {
      return x->float32Value == y->float32Value;
    } break;
    case Shizu_Value_Tag_Float64: {
      return x->float64Value == y->float64Value;
    } break;
    case Shizu_Value_Tag_Integer32: {
      return x->integer32Value == y->integer32Value;
    } break;
    case Shizu_Value_Tag_Integer64: {
      return x->integer64Value == y->integer64Value;
    } break;
    case Shizu_Value_Tag_Type: {
      return x->typeValue == y->typeValue;
    } break;
    case Shizu_Value_Tag_Void: {
      return Shizu_Boolean_True;
    } break;
    case Shizu_Value_Tag_Object:
    default: {
      Shizu_unreachableCodeReached(__FILE__, __LINE__);
    } break;
  };
}

Shizu_Integer32
Shizu_Value_getHashValue
  (
    Shizu_State2* state,
    Shizu_Value const* x
  )
{
  switch (x->tag) {
    case Shizu_Value_Tag_Boolean: {
      if (x->booleanValue) {
        return 1231;
      } else {
        return 1237;
      }
    } break;
    case Shizu_Value_Tag_CxxFunction: {
      return (intptr_t)(uintptr_t)x->cxxFunctionValue;
    } break;
    case Shizu_Value_Tag_Float32: {
      Shizu_Float32 y = x->float32Value;
      if (0.0f != y) {
        if (isnan(y)) {
          // Any nan maps to -3.
          return -3;
        } else {
          return hashBytes(state, (uint8_t const*)&y, sizeof(y));
        }
      } else {
        // Both -0 and +0 map to 0.
        return 0;
      }
    } break;
    case Shizu_Value_Tag_Float64: {
      Shizu_Float64 y = x->float64Value;
      if (0.0f != y) {
        if (isnan(y)) {
          // Any nan maps to -3.
          return -3;
        } else {
          return hashBytes(state, (uint8_t const*)&y, sizeof(y));
        }
      } else {
        // Both -0 and +0 map to 0.
        return 0;
      }
    } break;
    case Shizu_Value_Tag_Integer32: {
      return (Shizu_Integer32)x->integer32Value;
    } break;
    case Shizu_Value_Tag_Integer64: {
      return (Shizu_Integer32)x->integer64Value;
    } break;
    case Shizu_Value_Tag_Object: {
      return Shizu_Object_getHashValue(state, x->objectValue);
    } break;
    case Shizu_Value_Tag_Type: {
      return Shizu_Types_getTypeHash(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), x->typeValue);
    } break;
    case Shizu_Value_Tag_Void: {
      return 0;
    } break;
    default: {
      Shizu_unreachableCodeReached(__FILE__, __LINE__);
    } break;
  };
}
