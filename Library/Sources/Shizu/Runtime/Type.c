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
#include "Shizu/Runtime/Type.h"

#include "Shizu/Runtime/Type.private.h"

bool
Shizu_Type_isObjectType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(NULL != state1);
  Shizu_debugAssert(NULL != x);
  return (Shizu_TypeFlags_ObjectType == (Shizu_TypeFlags_ObjectType & x->flags));
}

bool
Shizu_Type_isPrimitiveType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(NULL != state1);
  Shizu_debugAssert(NULL != x);
  return (Shizu_TypeFlags_PrimitiveType == (Shizu_TypeFlags_PrimitiveType & x->flags));
}

Shizu_ObjectTypeDescriptor const*
Shizu_Type_getObjectTypeDescriptor
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(!Shizu_Type_isObjectType(state, self, x));
  return x->objectType.descriptor;
}

Shizu_PrimitiveTypeDescriptor const*
Shizu_Type_getPrimitiveTypeDescriptor
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(!Shizu_Type_isPrimitiveType(state, self, x));
  return x->primitiveType.descriptor;
}

bool
Shizu_Types_isSubTypeOf
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & x->flags)) {
    // "x" cannot be subtype of any type except of itself.
    return x == y;
  }
  if (0 == (Shizu_TypeFlags_ObjectType & y->flags)) {
    // "y" cannot be supertype of any type except of itself.
    return x == y;
  }
  Shizu_Type const* z = x;
  do {
    if (z == y) {
      break;
    }
    z = z->objectType.parentType;
  } while (z);
  return NULL != z;
}

bool
Shizu_Types_isTrueSubTypeOf
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & x->flags)) {
    // "x" cannot be subtype of any type except of itself.
    // However, not type can be a true subtype of itself.
    return false;
  }
  if (0 == (Shizu_TypeFlags_ObjectType & y->flags)) {
    // "y" cannot be supertype of any type except of itself.
    // However, not type can be a true supertype of itself.
    return false;
  }

  Shizu_Type const* z = x;
  do {
    if (z->objectType.parentType == y) {
      break;
    }
    z = z->objectType.parentType;
  } while (z);
  return NULL != z;
}

Shizu_Type*
Shizu_Types_getParentType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & x->flags)) {
    return NULL;
  } else {
    return x->objectType.parentType;
  }
}

void
Shizu_Types_getTypeName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x,
    char const** bytes,
    size_t* numberOfBytes
  )
{
  *bytes = x->name.bytes;
  *numberOfBytes = x->name.numberOfBytes;
}

Shizu_Object_Dispatch*
Shizu_Types_getDispatch
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & x->flags)) {
    return NULL;
  } else {
    return x->objectType.dispatch;
  }
}
