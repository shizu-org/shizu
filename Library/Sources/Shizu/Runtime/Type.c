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
#include "Shizu/Runtime/Type.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Type.private.h"

bool
Shizu_Types_isObjectType
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
Shizu_Types_isPrimitiveType
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

bool
Shizu_Types_isEnumerationType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(NULL != state1);
  Shizu_debugAssert(NULL != x);
  return (Shizu_TypeFlags_EnumerationType == (Shizu_TypeFlags_EnumerationType & x->flags));
}

bool
Shizu_Types_isSubTypeOf
  (
    Shizu_State1* state,
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
    Shizu_State1* state,
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

Shizu_ObjectTypeDescriptor const*
Shizu_Type_getObjectTypeDescriptor
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(Shizu_Types_isObjectType(state, self, x));
  return x->objectType.descriptor;
}

Shizu_PrimitiveTypeDescriptor const*
Shizu_Type_getPrimitiveTypeDescriptor
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  )
{
  Shizu_debugAssert(Shizu_Types_isPrimitiveType(state, self, x));
  return x->primitiveType.descriptor;
}

Shizu_Type*
Shizu_Types_getParentType
  (
    Shizu_State1* state,
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
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x,
    uint8_t const** bytes,
    Shizu_Integer32* numberOfBytes
  )
{
  *bytes = x->name.bytes;
  *numberOfBytes = x->name.numberOfBytes;
}

Shizu_Integer32
Shizu_Types_getTypeHash
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x
  )
{
  return x->name.hashValue;
}

Shizu_Object_Dispatch*
Shizu_Types_getDispatch
  (
    Shizu_State1* state,
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

typedef struct Float32 {

  Shizu_Integer32 precision;

  Shizu_Integer32 fractionBitsCount;
  Shizu_Integer32 fractionBitsShift;
  Shizu_Integer32 fractionBitsMask;

  Shizu_Integer32 exponentBitsCount;
  Shizu_Integer32 exponentBitsShift;
  Shizu_Integer32 exponentBitsMask;

  Shizu_Integer32 signBitsCount;
  Shizu_Integer32 signBitsShift;
  Shizu_Integer32 signBitsMask;

  // This approach is ok for single-threaded environments.
  struct {
    char* p;
    size_t n;
  } buffer;

} Float32;

static const char Float32NamedStorageNamed[] = "Shizu.Float32";

static void
Shizu_Float32_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_Float32_preDestroyType
  (
    Shizu_State1* state1
  );

static Shizu_PrimitiveTypeDescriptor const Shizu_Float32_Type = {
  .postCreateType = &Shizu_Float32_postCreateType,
  .preDestroyType = &Shizu_Float32_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_Float32),
};

static void
Shizu_Float32_postCreateType
  (
    Shizu_State1* state
  )
{
  if (Shizu_State1_allocateNamedStorage(state, Float32NamedStorageNamed, sizeof(Float32))) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_deallocateNamedStorage(state, Float32NamedStorageNamed);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  g->precision = Shizu_Float32_FractionBits + 1;
  // W := S - P := 32 - 24 = 8 
  g->fractionBitsCount = Shizu_Float32_FractionBits;
  g->fractionBitsShift = 0;
  g->fractionBitsMask = (1 << g->fractionBitsCount) - 1;         // 00000000011111111111111111111111
  g->fractionBitsMask <<= g->fractionBitsShift;

  g->exponentBitsCount = Shizu_Float32_ExponentBits;
  g->exponentBitsShift = g->fractionBitsCount;
  g->exponentBitsMask = (1 << g->exponentBitsCount) - 1;
  g->exponentBitsMask <<= g->exponentBitsShift;                 // 01111111100000000000000000000000

  g->signBitsCount = Shizu_Float32_SignBits;
  g->signBitsShift = g->exponentBitsCount + g->fractionBitsCount;
  g->signBitsMask = (1 << g->signBitsCount) - 1;
  g->signBitsMask <<= g->signBitsShift;                         // 10000000000000000000000000000000

  g->buffer.p = Shizu_State1_allocate(state, 1024 + 1);
  if (!g->buffer.p) {
    Shizu_State1_deallocateNamedStorage(state, Float32NamedStorageNamed);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  g->buffer.n = 1024 + 1;
}

static void
Shizu_Float32_preDestroyType
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    /*Unable to recover.*/
  }
  Shizu_State1_deallocate(state, g->buffer.p);
  g->buffer.p = NULL;
  g->buffer.n = 0;
  Shizu_State1_deallocateNamedStorage(state, Float32NamedStorageNamed);
}

Shizu_definePrimitiveType(Shizu_Float32);

Shizu_Integer32
Shizu_Float32_getBias
  (
    Shizu_State1* state
  )
{ return 127; }

Shizu_Integer32
Shizu_Float32_getUnbiasedMinimumExponent
  (
    Shizu_State1* state
  )
{ return Shizu_Float32_getBiasedMinimumExponent(state) - Shizu_Float32_getBias(state); }

Shizu_Integer32
Shizu_Float32_getUnbiasedMaximumExponent
  (
    Shizu_State1* state
  )
{ return Shizu_Float32_getBiasedMaximumExponent(state) - Shizu_Float32_getBias(state); }

Shizu_Integer32
Shizu_Float32_getBiasedMinimumExponent
  (
    Shizu_State1* state
  )
{ return 1; }

Shizu_Integer32
Shizu_Float32_getBiasedMaximumExponent
  (
    Shizu_State1* state
  )
{ return 254; }

Shizu_Integer32
Shizu_Float32_getExponentBitsMask
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->exponentBitsMask;
}

Shizu_Integer32
Shizu_Float32_getExponentBitsShift
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->exponentBitsShift;
}

Shizu_Integer32
Shizu_Float32_getSignBitsMask
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->signBitsMask;
}

Shizu_Integer32
Shizu_Float32_getSignBitsShift
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->signBitsShift;
}

Shizu_Integer32
Shizu_Float32_getFractionBitsMask
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->fractionBitsMask;
}

Shizu_Integer32
Shizu_Float32_getFractionBitsShift
  (
    Shizu_State1* state
  )
{
  Float32* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float32NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->fractionBitsMask;
}

#if 1 == Shizu_Configuration_WithFloat64

typedef struct Float64 {

  Shizu_Integer64 precision;

  Shizu_Integer64 fractionBitsCount;
  Shizu_Integer64 fractionBitsShift;
  Shizu_Integer64 fractionBitsMask;

  Shizu_Integer64 exponentBitsCount;
  Shizu_Integer64 exponentBitsShift;
  Shizu_Integer64 exponentBitsMask;

  Shizu_Integer64 signBitsCount;
  Shizu_Integer64 signBitsShift;
  Shizu_Integer64 signBitsMask;

  // This approach is ok for single-threaded environments.
  struct {
    char* p;
    size_t n;
  } buffer;

} Float64;

static const char Float64NamedStorageNamed[] = "Shizu.Float64";

static void
Shizu_Float64_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_Float64_preDestroyType
  (
    Shizu_State1* state1
  );

static Shizu_PrimitiveTypeDescriptor const Shizu_Float64_Type = {
  .postCreateType = &Shizu_Float64_postCreateType,
  .preDestroyType = &Shizu_Float64_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_Float64),
};

static void
Shizu_Float64_postCreateType
  (
    Shizu_State1* state
  )
{
  if (Shizu_State1_allocateNamedStorage(state, Float64NamedStorageNamed, sizeof(Float64))) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_deallocateNamedStorage(state, Float64NamedStorageNamed);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }

  g->precision = Shizu_Float64_FractionBits + 1;

  g->fractionBitsCount = Shizu_Float64_FractionBits;
  g->fractionBitsShift = 0;
  g->fractionBitsMask = (1 << g->fractionBitsCount) - 1;
  g->fractionBitsMask <<= g->fractionBitsShift;
  
  g->exponentBitsCount = Shizu_Float64_ExponentBits;
  g->exponentBitsShift = g->fractionBitsCount;
  g->exponentBitsMask = (1 << g->exponentBitsCount) - 1;
  g->exponentBitsMask <<= g->exponentBitsShift;                 

  g->signBitsCount = Shizu_Float64_SignBits;
  g->signBitsShift = g->exponentBitsCount + g->fractionBitsCount;
  g->signBitsMask = (1 << g->signBitsCount) - 1;
  g->signBitsMask <<= g->signBitsShift;

  g->buffer.p = Shizu_State1_allocate(state, 1024+1);
  if (!g->buffer.p) {
    Shizu_State1_deallocateNamedStorage(state, Float64NamedStorageNamed);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  g->buffer.n = 1024 + 1;
}

static void
Shizu_Float64_preDestroyType
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    /*Unable to recover.*/
  }
  Shizu_State1_deallocate(state, g->buffer.p);
  g->buffer.p = NULL;
  g->buffer.n = 0;
  Shizu_State1_deallocateNamedStorage(state, Float64NamedStorageNamed);
}

Shizu_definePrimitiveType(Shizu_Float64);

Shizu_Integer64
Shizu_Float64_getBias
  (
    Shizu_State1* state
  )
{ return 1023; }

Shizu_Integer64
Shizu_Float64_getUnbiasedMinimumExponent
(
  Shizu_State1* state
)
{ return Shizu_Float64_getBiasedMinimumExponent(state) - Shizu_Float64_getBias(state); }

Shizu_Integer64
Shizu_Float64_getUnbiasedMaximumExponent
(
  Shizu_State1* state
)
{ return Shizu_Float64_getBiasedMaximumExponent(state) - Shizu_Float64_getBias(state); }

Shizu_Integer64
Shizu_Float64_getBiasedMinimumExponent
  (
    Shizu_State1* state
  )
{ return 1; }

Shizu_Integer64
Shizu_Float64_getBiasedMaximumExponent
  (
    Shizu_State1* state
  )
{ return 2046; }

Shizu_Integer64
Shizu_Float64_getExponentBitsMask
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->exponentBitsMask;
}

Shizu_Integer64
Shizu_Float64_getExponentBitsShift
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->exponentBitsShift;
}

Shizu_Integer64
Shizu_Float64_getSignBitsMask
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->signBitsMask;
}

Shizu_Integer64
Shizu_Float64_getSignBitsShift
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->signBitsShift;
}

Shizu_Integer64
Shizu_Float64_getFractionBitsMask
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->fractionBitsMask;
}

Shizu_Integer64
Shizu_Float64_getFractionBitsShift
  (
    Shizu_State1* state
  )
{
  Float64* g = NULL;
  if (Shizu_State1_getNamedStorage(state, Float64NamedStorageNamed, (void**)&g)) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  return g->fractionBitsShift;
}

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
