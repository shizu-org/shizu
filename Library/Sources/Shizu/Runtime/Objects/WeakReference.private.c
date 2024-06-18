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
#include "Shizu/Runtime/Objects/WeakReference.private.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"

// malloc, free
#include <malloc.h>

// memcmp, memcpy
#include <string.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "idlib/bit_scan.h"

static void
Shizu_WeakReference_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_WeakReference_preDestroyType
  (
    Shizu_State1* state1
  );

static void
Shizu_WeakReference_finalize
  (
    Shizu_State2* state,
    Shizu_WeakReference* self
  );

static size_t
hashPointer
  (
    void* p
  );

struct Shizu_WeakReference_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Shizu_WeakReference {
  Shizu_Object _parent;
  Shizu_WeakReference* next;
  /// @brief A pointer to the Shizu_Object value or the null pointer.
  Shizu_Object* reference;
};

static Shizu_TypeDescriptor const Shizu_WeakReference_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_WeakReference_postCreateType,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_WeakReference_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_WeakReference),
  .visit = NULL,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_WeakReference_finalize,
  .dispatchSize = sizeof(Shizu_WeakReference_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_WeakReference, Shizu_Object);

typedef struct WeakReferenceNode WeakReferenceNode;

struct WeakReferenceNode {
	WeakReferenceNode* next;
	/// @brief A pointer to the object or the null pointer.
	Shizu_Object* reference;
	/// @brief The hash value of the address of the managed object.
	size_t hashValue;
	/// @brief A pointer to the weak reference.
	Shizu_WeakReference* weakReference;
};

struct Shizu_WeakReferences {
	Shizu_WeakReference** buckets;
	size_t size;
	size_t capacity;
  size_t minimalCapacity;
	size_t maximalCapacity;
};

Shizu_WeakReferences*
Shizu_WeakReferences_create
  (
    Shizu_State1* state
  )
{
  // (1)
  Shizu_WeakReferences* self = Shizu_State1_allocate(state, sizeof(Shizu_WeakReferences));
  if (!self) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  // (2)
  self->minimalCapacity = 8;
  self->maximalCapacity = SIZE_MAX / sizeof(WeakReferenceNode*);
  if (self->maximalCapacity > Shizu_Integer32_Maximum) {
    self->maximalCapacity = Shizu_Integer32_Maximum;
  }
  if (self->maximalCapacity < self->minimalCapacity) {
    Shizu_State1_deallocate(state, self);
    self = NULL;
    Shizu_State1_setStatus(state, 1);
    Shizu_State1_jump(state);
  }
  // (3)
  self->capacity = self->minimalCapacity;
  self->buckets = Shizu_State1_allocate(state, sizeof(Shizu_WeakReference*) * self->capacity);
  if (!self->buckets) {
    Shizu_State1_deallocate(state, self);
    self = NULL;
    Shizu_State1_setStatus(state, 1);
    Shizu_State1_jump(state);
  }
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  // (4)
  self->size = 0;
  // (5)
  return self;
}

void
Shizu_WeakReferences_destroy
  (
    Shizu_State1* state,
    Shizu_WeakReferences* self
  )
{
  // @todo In debug mode, assert the table is empty.
  self->size = 0;
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_WeakReference** bucket = &(self->buckets[i]);
    while (*bucket) {
      Shizu_WeakReference* node = *bucket;
      *bucket = (*bucket)->next;
      Shizu_State1_deallocate(state, node);
    }
  }
  self->capacity = 0;
  Shizu_State1_deallocate(state, self->buckets);
  self->buckets = NULL;
  Shizu_State1_deallocate(state, self);
  self = NULL;
}

void
Shizu_WeakReferenceState_notifyObjectFinalize
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_WeakReferences* self,
    Shizu_Object* reference
  )
{
  // Remove all weak references to that object from the hash table.
  // Set their reference field to null.
  // Remove the weak reference from the hash table.
  size_t hashValue = hashPointer(reference);
  size_t hashIndex = hashValue % self->capacity;
  Shizu_WeakReference** previous = &(self->buckets[hashIndex]);
  Shizu_WeakReference* current = self->buckets[hashIndex];
  while (current) {
    if (current->reference == reference) {
      Shizu_WeakReference* weakReference = current;
      *previous = current->next;
      current = current->next;
      weakReference->next = NULL;
      self->size--;
      break;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
}

static void
Shizu_WeakReference_postCreateType
  (
    Shizu_State1* state1
  )
{/*Intentionally empty.*/}

static void
Shizu_WeakReference_preDestroyType
  (
    Shizu_State1* state1
  )
{/*Intentionally empty.*/}

static void
Shizu_WeakReference_finalize
  (
    Shizu_State2* state,
    Shizu_WeakReference* self
  )
{
  Shizu_WeakReferences* g = Shizu_State2_getWeakReferences(state);
  if (!g) {
    fprintf(stderr, "%s: %d: error: weak references not initialized\n", __FILE__, __LINE__);
    Shizu_State2_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State2_jump(state);
  }
  // Remove the weak reference from the hash table.
  size_t hashValue = hashPointer(self->reference);
  size_t hashIndex = hashValue % g->capacity;
  Shizu_WeakReference** previous = &(g->buckets[hashIndex]);
  Shizu_WeakReference* current = g->buckets[hashIndex];
  while (current) {
    if (current == self) {
      *previous = current->next;
      self->next = NULL;
      g->size--;
      break;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
}

static size_t
hashPointer
  (
    void* p
  )
{ return (size_t)(uintptr_t)p; }

void
Shizu_WeakReference_construct
  (
    Shizu_State2* state,
    Shizu_WeakReference* self,
    Shizu_Object* reference
  )
{
  Shizu_Type* TYPE = Shizu_WeakReference_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  Shizu_WeakReferences* g = Shizu_State2_getWeakReferences(state);
  // Add the weak reference to the hash table.
  size_t hashValue = hashPointer(reference);
  size_t hashIndex = hashValue % g->capacity;
  self->reference = reference;
  self->next = g->buckets[hashIndex];
  g->buckets[hashIndex] = self;
  g->size++;
  ((Shizu_Object*)self)->type = TYPE;
}

Shizu_WeakReference*
Shizu_WeakReference_create
  (
    Shizu_State2* state,
    Shizu_Object* reference
  )
{
  Shizu_Type* TYPE = Shizu_WeakReference_getType(state);
  Shizu_WeakReference* self = (Shizu_WeakReference*)Shizu_Gc_allocateObject(state, sizeof(Shizu_WeakReference));
  Shizu_WeakReference_construct(state, self, reference);
  return self;
}
