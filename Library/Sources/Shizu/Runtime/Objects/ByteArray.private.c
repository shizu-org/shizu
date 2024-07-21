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
#include "Shizu/Runtime/Objects/ByteArray.private.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"

// memcmp, memcpy
#include <string.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "idlib/bit_scan.h"

static size_t
getNewBestCapacity
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t additionalCapacity
  );

static void
Shizu_ByteArray_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_ByteArray_preDestroyType
  (
    Shizu_State1* state1
  );

static void
Shizu_ByteArray_visit
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  );

static void
Shizu_ByteArray_finalize
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  );

static void
Shizu_ByteArray_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Shizu_ByteArray_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_ByteArray_postCreateType,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_ByteArray_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_ByteArray),
  .construct = &Shizu_ByteArray_constructImpl,
  .visit = (Shizu_OnVisitCallback*) & Shizu_ByteArray_visit,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_ByteArray_finalize,
  .dispatchSize = sizeof(Shizu_ByteArray_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

static const char* namedMemoryName = "Shizu.ByteArray.NamedMemory";

typedef struct ByteArrays {
  Shizu_Integer32 maximumPowerOfTwoInteger32;
  size_t maximumPowerOfTwoSz;
  size_t minimumCapacity;
  size_t maximumCapacity;
} ByteArrays;

static size_t
getNewBestCapacity
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t additionalCapacity
  )
{
  if (!additionalCapacity) {
    return self->capacity;
  }

  ByteArrays* g = NULL;
  if (Shizu_State1_getNamedStorage(Shizu_State2_getState1(state), namedMemoryName, (void**)&g)) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  size_t oldCapacity = self->capacity;
  size_t newCapacity;
  if (g->maximumCapacity - oldCapacity >= additionalCapacity) {
    newCapacity = oldCapacity + additionalCapacity;
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  int result = idlib_power_of_two_ge_sz(&newCapacity, newCapacity);
  if (result) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  return newCapacity;
}

static void
Shizu_ByteArray_postCreateType
  (
    Shizu_State1* state1
  )
{
  if (Shizu_State1_allocateNamedStorage(state1, namedMemoryName, sizeof(ByteArrays))) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }

  ByteArrays* g = NULL;
  if (Shizu_State1_getNamedStorage(state1, namedMemoryName, (void**)&g)) {
    Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }

  g->minimumCapacity = 8;

  g->maximumPowerOfTwoSz = SIZE_MAX;
  while (g->maximumPowerOfTwoSz % 2 != 0) {
    g->maximumPowerOfTwoSz--;
  }

  g->maximumPowerOfTwoInteger32 = Shizu_Integer32_Maximum;
  while (g->maximumPowerOfTwoInteger32 % 2 != 0) {
    g->maximumPowerOfTwoInteger32--;
  }

  g->maximumCapacity = g->maximumPowerOfTwoInteger32 < g->maximumPowerOfTwoSz ? g->maximumPowerOfTwoInteger32 : g->maximumPowerOfTwoSz;
}

static void
Shizu_ByteArray_preDestroyType
  (
    Shizu_State1* state1
  )
{
  Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
}

static void
Shizu_ByteArray_visit
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  )
{ }

static void
Shizu_ByteArray_finalize
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  )
{
  self->size = 0;
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self->elements);
  self->elements = NULL;
  self->capacity = 0;
}

static void
Shizu_ByteArray_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isObject(&argumentValues[0])) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ByteArray* SELF = (Shizu_ByteArray*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Type* TYPE = Shizu_ByteArray_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->elements = Shizu_State1_allocate(Shizu_State2_getState1(state), 8);
  if (!SELF->elements) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  SELF->size = 0;
  SELF->capacity = 8;
  ((Shizu_Object*)SELF)->type = TYPE;
}

Shizu_defineObjectType("Shizu.ByteArray", Shizu_ByteArray, Shizu_Object);

Shizu_ByteArray*
Shizu_ByteArray_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Shizu_ByteArray_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_ByteArray* SELF = (Shizu_ByteArray*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void), };
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)SELF);
  DESCRIPTOR->construct(state, &returnValue, 1, &(argumentValues[0]));
  return SELF;
}

static Shizu_Value const IndexOutOfBounds = Shizu_Value_InitializerVoid(Shizu_Void_Void);

Shizu_Value
Shizu_ByteArray_getValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t index
  )
{
  if (index >= self->size) {
    return IndexOutOfBounds;
  }
  Shizu_Value temporary;
  Shizu_Value_setInteger32(&temporary, self->elements[index]);
  return temporary;
}

Shizu_Value
Shizu_ByteArray_getSize
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  )
{
  Shizu_Value value;
  Shizu_Value_setInteger32(&value, (Shizu_Integer32)self->size);
  return value;
}

void
Shizu_ByteArray_insertValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t index,
    Shizu_Value const* value
  )
{
  if (Shizu_Value_isVoid(value)) {
    return;
  }
  if (!Shizu_Value_isInteger32(value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (Shizu_Value_getInteger32(value) < 0 || Shizu_Value_getInteger32(value) > UINT8_MAX) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentOutOfRange);
    Shizu_State2_jump(state);
  }
  if (self->capacity == self->size) {
    size_t newCapacity = getNewBestCapacity(state, self, 1);
    uint8_t* newElements = realloc(self->elements, newCapacity * sizeof(uint8_t));
    if (!newElements) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    self->elements = newElements;
    self->capacity = newCapacity;
  }
  if (index < self->size) {
    memmove(self->elements + index,
            self->elements + index + 1,
            sizeof(uint8_t) * (self->size - index));
  }

  self->elements[index] = (uint8_t)Shizu_Value_getInteger32(value);
  self->size++;
}

void
Shizu_ByteArray_appendValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Value const* value
  )
{
  Shizu_ByteArray_insertValue(state, self, self->size, value);
}

void
Shizu_ByteArray_prependValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Value const* value
  )
{
  Shizu_ByteArray_insertValue(state, self, 0, value);
}

void
Shizu_ByteArray_insertRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t i,
    void const* p,
    size_t n
  )
{
  if (self->capacity - self->size < n) {
    size_t newCapacity = getNewBestCapacity(state, self, n - (self->capacity - self->size));
    uint8_t* newElements = realloc(self->elements, newCapacity * sizeof(uint8_t));
    if (!newElements) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    self->elements = newElements;
    self->capacity = newCapacity;
  }
  if (i < self->size) {
    memmove(self->elements + i,
            self->elements + i + 1,
            sizeof(uint8_t) * (self->size - i));
  }
  memcpy(self->elements + i, p, n);
  self->size += n;
}

void
Shizu_ByteArray_prependRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    void const* p,
    size_t n
  )
{
  Shizu_ByteArray_insertRawBytes(state, self, 0, p, n);
}

void
Shizu_ByteArray_appendRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    void const* p,
    size_t n
  )
{
  Shizu_ByteArray_insertRawBytes(state, self, self->size, p, n);
}

void*
Shizu_ByteArray_getRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  )
{ return self->elements; }

size_t
Shizu_ByteArray_getNumberOfRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  )
{ return self->size; }

Shizu_Boolean
Shizu_ByteArray_compareRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray const* self,
    Shizu_ByteArray const* other
  )
{
  if (self == other) {
    return true;
  }
  if (self->size != other->size) {
    return false;
  }
  return !memcmp(self->elements, other->elements, self->size);
}

void
Shizu_ByteArray_clear
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  ) 
{
  self->size = 0;
}

void
Shizu_ByteArray_removeFront
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Integer32 count
  )
{
  if (count < 0 || count > self->size) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  memmove(self->elements, self->elements + count, self->size - count);
  self->size -= count;
}

void
Shizu_ByteArray_removeBack
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Integer32 count
  )
{
  if (count < 0 || count > self->size) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  self->size -= count;
}

void
Shizu_ByteArray_trimLeading
(
  Shizu_State2* state,
  Shizu_ByteArray* a,
  Shizu_Integer32 v
) {
  uint8_t* p = Shizu_ByteArray_getRawBytes(state, a);
  Shizu_Integer32 n = (Shizu_Integer32)Shizu_ByteArray_getNumberOfRawBytes(state, a);
  Shizu_Integer32 count = 0; // number of consecutive occurrences of v from the left side of the string.
  for (Shizu_Integer32 i = 0; i < n; ++i) {
    if (v == p[i]) {
      count++;
    } else {
      break;
    }
  }
  if (count) {
    Shizu_ByteArray_removeFront(state, a, count);
  }
}

void
Shizu_ByteArray_trimTrailing
  (
    Shizu_State2* state,
    Shizu_ByteArray* a,
    Shizu_Integer32 v
  )
{
  uint8_t* p = Shizu_ByteArray_getRawBytes(state, a);
  Shizu_Integer32 n = (Shizu_Integer32)Shizu_ByteArray_getNumberOfRawBytes(state, a);
  Shizu_Integer32 count = 0; // number of consecutive occurrences of v from the right side of the string.
  for (Shizu_Integer32 i = n; i > 0; --i) {
    if (v == p[i]) {
      count++;
    } else {
      break;
    }
  }
  if (count) {
    Shizu_ByteArray_removeBack(state, a, count);
  }
}

void
Shizu_ByteArray_reverse
  (
    Shizu_State2* state,
    Shizu_ByteArray* a
  )
{
  if (a->size < 2) {
    return;
  }
  for (size_t i = 0, n = a->size / 2; i < n; ++i) {
    size_t j = a->size - i - 1;
    uint8_t t = a->elements[j];
    a->elements[j] = a->elements[i];
    a->elements[i] = t;
  }
}
