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
#include "Shizu/Runtime/Objects/Map.private.h"


#include "Shizu/Runtime/State.h"
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

// INT_MAX, SIZE_MAX
#include <limits.h>

#include "idlib/bit_scan.h"

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
    Shizu_State* state,
    Shizu_Map* self
  );

static void
Shizu_Map_finalize
  (
    Shizu_State* state,
    Shizu_Map* self
  );

static Shizu_TypeDescriptor const Shizu_Map_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_Map_postCreateType,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_Map_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_Map),
  .visit = (Shizu_OnVisitCallback*) & Shizu_Map_visit,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_Map_finalize,
  .dispatchSize = sizeof(Shizu_Map_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

static const char* namedMemoryName = "Shizu.Maps.NamedMemory";

typedef struct Maps {
  size_t minimumCapacity;
  size_t maximumCapacity;
} Maps;

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
  g->minimumCapacity = 8;
  g->maximumCapacity = SIZE_MAX / sizeof(Shizu_Map_Node*);
  if (g->maximumCapacity > Shizu_Integer32_Maximum) {
    g->maximumCapacity = Shizu_Integer32_Maximum;
  }
  if (g->maximumCapacity > INT_MAX) {
    g->maximumCapacity = INT_MAX;
  }
  if (g->maximumCapacity > SIZE_MAX) {
    g->maximumCapacity = SIZE_MAX;
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
    Shizu_State* state,
    Shizu_Map* self
  )
{
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
		Shizu_Map_Node* node = self->buckets[i];
		while (node) {
			Shizu_Gc_visitValue(Shizu_State_getState1(state), Shizu_State_getGc(state), &node->key);
			Shizu_Gc_visitValue(Shizu_State_getState1(state), Shizu_State_getGc(state), &node->value);
			node = node->next;
		}
	}
}

void
Shizu_Map_finalize
  (
		Shizu_State* state,
		Shizu_Map* self
	)
{
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
		Shizu_Map_Node** bucket = &(self->buckets[i]);
		while (*bucket) {
			Shizu_Map_Node* node = *bucket;
			*bucket = node->next;
			free(node);
		}
	}
	free(self->buckets);
  self->buckets = NULL;
}

Shizu_defineType(Shizu_Map, Shizu_Object);

void
Shizu_Map_construct
  (
    Shizu_State* state,
    Shizu_Map* self
  )
{
  Shizu_Type* TYPE = Shizu_Map_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->buckets = malloc(sizeof(Shizu_Map_Node*) * 8);
  if (!self->buckets) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  self->size = 0;
  self->capacity = 8;
  ((Shizu_Object*)self)->type = TYPE;
}

Shizu_Map*
Shizu_Map_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Shizu_Map_getType(state);
  Shizu_Map* self = (Shizu_Map*)Shizu_Gc_allocateObject(state, sizeof(Shizu_Map));
  Shizu_Map_construct(state, self);
  return self;
}

size_t
Shizu_Map_getSize
  (
    Shizu_State* state,
    Shizu_Map* self
  )
{ return self->size; }
