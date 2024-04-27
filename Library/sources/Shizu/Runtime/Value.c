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

#include "Shizu/Runtime/DebugAssert.h"

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

Shizu_Reference(Shizu_CxxFunction)
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
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue
  )
{
  Shizu_debugAssert(NULL != self);
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

Shizu_Reference(Shizu_Object)
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
    Shizu_Reference(Shizu_Object) objectValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->objectValue = objectValue;
  self->tag = Shizu_Value_Tag_Object;
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
