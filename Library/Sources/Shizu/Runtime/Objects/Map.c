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
#include "Shizu/Runtime/Objects/Map.private.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"
#include "Shizu/Runtime/countLeadingZeroes.h"

// INT_MAX, SIZE_MAX
#include <limits.h>

static const char* namedMemoryName = "Shizu.Maps.NamedMemory";

typedef struct Maps {
  Shizu_Integer32 minimumCapacity;
  Shizu_Integer32 maximumCapacity;
} Maps;

static void
Shizu_Map_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_Map_preDestroyType
  (
    Shizu_State1* state1
  );

static void
Shizu_Map_visit
  (
    Shizu_State2* state,
    Shizu_Map* self
  );

static void
Shizu_Map_finalize
  (
    Shizu_State2* state,
    Shizu_Map* self
  );

static void
Shizu_Map_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static void
optimize
  (
    Shizu_State2* state,
    Shizu_Map* self
  )
{
  float load = (float)self->size / (float)self->capacity;
  if (load > 0.75f) {
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Maps* g = NULL;
      if (Shizu_State1_getNamedStorage(Shizu_State2_getState1(state), namedMemoryName, (void**)&g)) {
        Shizu_State1_setStatus(Shizu_State2_getState1(state), Shizu_Status_AllocationFailed);
        Shizu_State1_jump(Shizu_State2_getState1(state));
      }
      if (self->capacity == g->minimumCapacity) {
        return;
      }
      Shizu_Integer32 oldCapacity = self->capacity;
      Shizu_Integer32 newCapacity = self->capacity * 2;
      Shizu_Map_Node** oldBuckets = self->buckets;
      Shizu_Map_Node** newBuckets = Shizu_State1_allocate(Shizu_State2_getState1(state), (size_t)newCapacity * sizeof(Shizu_Map_Node*));
      if (!newBuckets) {
        Shizu_State1_setStatus(Shizu_State2_getState1(state), Shizu_Status_AllocationFailed);
        Shizu_State1_jump(Shizu_State2_getState1(state));
      }
      for (Shizu_Integer32 i = 0, n = newCapacity; i < n; ++i) {
        newBuckets[i] = NULL;
      }
      for (Shizu_Integer32 i = 0, n = oldCapacity; i  < n; ++i) {
        while (oldBuckets[i]){
          Shizu_Map_Node* node = oldBuckets[i];
          oldBuckets[i] = node->next;
          Shizu_Integer32 hashValue = node->hashValue;
          Shizu_Integer32 hashIndex = (hashValue & 0x7FFFFFFF) % self->capacity;
          node->next = newBuckets[hashIndex];
          newBuckets[hashIndex] = node;
        }
      }
      Shizu_State1_deallocate(Shizu_State2_getState1(state), oldBuckets);
      self->capacity = newCapacity;
      self->buckets = newBuckets;
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
    }
  }
}

static Shizu_ObjectTypeDescriptor const Shizu_Map_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_Map_postCreateType,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_Map_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_Map),
  .construct = &Shizu_Map_constructImpl,
  .visit = (Shizu_OnVisitCallback*) & Shizu_Map_visit,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_Map_finalize,
  .dispatchSize = sizeof(Shizu_Map_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

static void
Shizu_Map_postCreateType
  (
    Shizu_State1* state1
  )
{
  if (Shizu_State1_allocateNamedStorage(state1, namedMemoryName, sizeof(Maps))) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  Maps* g = NULL;
  if (Shizu_State1_getNamedStorage(state1, namedMemoryName, (void**)&g)) {
    Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    size_t leadingZeroes;
    size_t shift;
    size_t t;

    // Determine minimum capacity.
    t = 8;
    if (t > Shizu_Integer32_Maximum) {
      t = Shizu_Integer32_Maximum;
    }
    if (t > INT_MAX) {
      t = INT_MAX;
    }
    g->minimumCapacity = t;
    leadingZeroes = Shizu_countLeadingZeroesU32(state1, (uint32_t)g->minimumCapacity);
    shift = 32 - 1 - leadingZeroes;
    t = ((size_t)1 << shift);
    g->minimumCapacity = t;
    // Determine maximum capacity.
    t = SIZE_MAX / sizeof(Shizu_Map_Node*);
    if (t > Shizu_Integer32_Maximum) {
      t = Shizu_Integer32_Maximum;
    }
    if (t > INT_MAX) {
      t = INT_MAX;
    }
    g->maximumCapacity = t;
    leadingZeroes = Shizu_countLeadingZeroesU32(state1, (uint32_t)g->maximumCapacity);
    shift = 32 - 1 - leadingZeroes;
    t = ((size_t)1 << shift);
    g->maximumCapacity = t;
    //
    if (g->minimumCapacity > g->maximumCapacity) {
      Shizu_State1_setStatus(state1, 1);
      Shizu_State1_jump(state1);
    }
    Shizu_State1_popJumpTarget(state1);
  } else {
    Shizu_State1_popJumpTarget(state1);
    Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
    Shizu_State1_jump(state1);
  }
}

static void
Shizu_Map_preDestroyType
  (
    Shizu_State1* state1
  )
{
  Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
}

static void
Shizu_Map_visit
  (
    Shizu_State2* state,
    Shizu_Map* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_Map_Node* node = self->buckets[i];
    while (node) {
      Shizu_Gc_visitValue(Shizu_State2_getState1(state), Shizu_State2_getGc(state), &node->key);
      Shizu_Gc_visitValue(Shizu_State2_getState1(state), Shizu_State2_getGc(state), &node->value);
      node = node->next;
    }
  }
}

void
Shizu_Map_finalize
  (
    Shizu_State2* state,
    Shizu_Map* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_Map_Node** bucket = &(self->buckets[i]);
    while (*bucket) {
      Shizu_Map_Node* node = *bucket;
      *bucket = node->next;
      Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
    }
  }
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self->buckets);
  self->buckets = NULL;
}

static void
Shizu_Map_constructImpl
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
  Shizu_Map* SELF = (Shizu_Map*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Type* TYPE = Shizu_Map_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->buckets = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Shizu_Map_Node*) * 8);
  if (!SELF->buckets) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    SELF->buckets[i] = NULL;
  }
  SELF->size = 0;
  SELF->capacity = 8;
  ((Shizu_Object*)SELF)->type = TYPE;
}

Shizu_defineObjectType("Shizu.Map", Shizu_Map, Shizu_Object);

void
Shizu_Map_clear
  (
    Shizu_State2* state,
    Shizu_Map* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    for (size_t i = 0, n = self->capacity; i < n; ++i) {
      Shizu_Map_Node** bucket = &(self->buckets[i]);
      while (*bucket) {
        Shizu_Map_Node* node = *bucket;
        *bucket = node->next;
        Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
      }
    }
  }
  self->size = 0;
}

Shizu_Integer32
Shizu_Map_getSize
  (
    Shizu_State2* state,
    Shizu_Map* self
  )
{
  Shizu_Cxx_Debug_assert(self->size >= 0);
  Shizu_Cxx_Debug_assert(self->size <= Shizu_Integer32_Maximum);
  return (Shizu_Integer32)self->size;
}

void
Shizu_Map_set
  (
    Shizu_State2* state,
    Shizu_Map* self,
    Shizu_Value* key,
    Shizu_Value* value
  )
{
  if (Shizu_Value_isVoid(key)) {
    return;
  }
  Shizu_Integer32 hashValue = Shizu_Value_getHashValue(state, key);
  Shizu_Integer32 hashIndex = (hashValue & 0x7FFFFFFF) % self->capacity;
  Shizu_Map_Node* node;
  for (node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue) {
      if (Shizu_Value_isEqualTo(state, &node->key, key)) {
        break;
      }
    }
  }
  if (node) {
    node->key = *key;
    node->value = *value;
  } else {
    node = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Shizu_Map_Node));
    if (!node) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    node->key = *key;
    node->value = *value;
    node->hashValue = hashValue;
    node->next = self->buckets[hashIndex];
    self->buckets[hashIndex] = node;
    self->size++;
    optimize(state, self);
  }
}

Shizu_Value
Shizu_Map_get
  (
    Shizu_State2* state,
    Shizu_Map* self,
    Shizu_Value* key
  )
{
  Shizu_Value result = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  if (Shizu_Value_isVoid(key)) {
    return result;
  }
  Shizu_Integer32 hashValue = Shizu_Value_getHashValue(state, key);
  Shizu_Integer32 hashIndex = (hashValue & 0x7FFFFFFF) % self->capacity;
  for (Shizu_Map_Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->hashValue == hashValue) {
      if (Shizu_Value_isEqualTo(state, &node->key, key)) {
        return node->value;
      }
    }
  }
  return result;
}
