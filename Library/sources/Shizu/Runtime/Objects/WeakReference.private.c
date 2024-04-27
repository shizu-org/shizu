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

#include "Shizu/Runtime/Gc.private.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static void
Shizu_WeakReference_staticInitialize
  (
    Shizu_State* state
  );

static void
Shizu_WeakReference_staticUninitialize
  (
    Shizu_State* state
  );

static void
Shizu_WeakReference_finalize
  (
    Shizu_State* state,
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
  .staticInitialize = (Shizu_OnStaticInitializeCallback*) & Shizu_WeakReference_staticInitialize,
  .staticFinalize = (Shizu_OnStaticFinalizeCallback*) & Shizu_WeakReference_staticUninitialize,
  .staticVisit = NULL,
  .size = sizeof(Shizu_WeakReference),
  .visit = NULL,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_WeakReference_finalize,
  .dispatchSize = sizeof(Shizu_WeakReference_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_WeakReference, Shizu_Object);

typedef struct WeakReferences WeakReferences;

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

struct WeakReferences {
	//* free;
	Shizu_WeakReference** buckets;
	size_t size;
	size_t capacity;
  size_t minimalCapacity;
	size_t maximalCapacity;
};

static const char* namedMemoryName = "Shizu.WeakReference.NamedMemory";

static void
Shizu_WeakReference_staticInitialize
  (
    Shizu_State* state
  )
{
  if (Shizu_State_allocateNamedMemory(state, namedMemoryName, sizeof(WeakReferences))) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  WeakReferences* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    Shizu_State_deallocateNamedMemory(state, namedMemoryName);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  // (2)
  g->minimalCapacity = 8;
	g->maximalCapacity = SIZE_MAX / sizeof(WeakReferenceNode*);
	if (g->maximalCapacity > Shizu_Integer32_Maximum) {
		g->maximalCapacity = Shizu_Integer32_Maximum;
	}
  if (g->maximalCapacity < g->minimalCapacity) {
    Shizu_State_deallocateNamedMemory(state, namedMemoryName);
    free(g);
    g = NULL;
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  // (3)
  g->capacity = g->minimalCapacity;
  g->buckets = malloc(sizeof(Shizu_WeakReference*) * g->capacity);
	if (!g->buckets) {
    Shizu_State_deallocateNamedMemory(state, namedMemoryName);
		free(g);
		g = NULL;
		Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
	}
	for (size_t i = 0, n = g->capacity; i < n; ++i) {
		g->buckets[i] = NULL;
	}

  // (4)
  g->size = 0;
}

static void
Shizu_WeakReference_staticUninitialize
  (
    Shizu_State* state
  )
{
  WeakReferences* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  // TODO: In debug mode, assert the table must be empty.
  // (4), (3)
  g->size = 0;
	for (size_t i = 0, n = g->capacity; i < n; ++i) {
    Shizu_WeakReference** bucket = &(g->buckets[i]);
		while (*bucket) {
      Shizu_WeakReference* node = *bucket;
			*bucket = (*bucket)->next;
			free(node);
		}
	}
  g->capacity = 0;
  free(g->buckets);
  g->buckets = NULL;

  // (2)
  Shizu_State_deallocateNamedMemory(state, namedMemoryName);
}

static void
Shizu_WeakReference_finalize
  (
    Shizu_State* state,
    Shizu_WeakReference* self
  )
{
  WeakReferences* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  //if (self->reference) {
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
  //}
}

static size_t
hashPointer
  (
    void* p
  )
{ return (size_t)(uintptr_t)p; }

Shizu_WeakReference*
Shizu_WeakReference_create
  (
    Shizu_State* state,
    Shizu_Object* reference
  )
{
  Shizu_Type* TYPE = Shizu_WeakReference_getType(state);
  WeakReferences* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  Shizu_WeakReference* self = (Shizu_WeakReference*)Shizu_Gc_allocate(state, sizeof(Shizu_WeakReference));

  // Add the weak reference to the hash table.
	size_t hashValue = hashPointer(reference);
	size_t hashIndex = hashValue % g->capacity;
  self->reference = reference;
  self->next = g->buckets[hashIndex];
  g->buckets[hashIndex] = self;
  g->size++;

  ((Shizu_Object*)self)->type = TYPE;
  return self;
}

void
Shizu_WeakReferences_notifyDestroy
	(
		Shizu_State* state,
		Shizu_Object* reference
	)
{
  WeakReferences* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    // This can happen if we are not yet initialized.
    return;
  }
  // Remove all weak references to that object from the hash table.
  // Set their reference field to null.
  // Remove the weak reference from the hash table.
  size_t hashValue = hashPointer(reference);
  size_t hashIndex = hashValue % g->capacity;
  Shizu_WeakReference** previous = &(g->buckets[hashIndex]);
  Shizu_WeakReference* current = g->buckets[hashIndex];
  while (current) {
    if (current->reference == reference) {
      Shizu_WeakReference* weakReference = current;
      *previous = current->next;
      current = current->next;
      weakReference->next = NULL;
      g->size--;
      break;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
}

Shizu_Object*
Shizu_WeakReference_get
  (
    Shizu_State* state,
    Shizu_WeakReference* self
  )
{ return self->reference; }
