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

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static void
Shizu_Map_staticInitialize
  (
    Shizu_State* state
  );

static void
Shizu_Map_staticFinalize
  ( 
    Shizu_State* state
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
  .staticInitialize = &Shizu_Map_staticInitialize,
  .staticFinalize = &Shizu_Map_staticFinalize,
  .staticVisit = NULL,
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
Shizu_Map_staticInitialize
  (
    Shizu_State* state
  )
{
  if (Shizu_State_allocateNamedMemory(state, namedMemoryName, sizeof(Maps))) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Maps* g = NULL;
  if (Shizu_State_getNamedMemory(state, namedMemoryName, &g)) {
    Shizu_State_deallocateNamedMemory(state, namedMemoryName);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  g->minimumCapacity = 8;
  g->maximumCapacity = SIZE_MAX / sizeof(Shizu_Map_Node*);
  if (g->maximumCapacity > Shizu_Integer32_Maximum) {
    g->maximumCapacity = Shizu_Integer32_Maximum;
  }
}

static void
Shizu_Map_staticFinalize
  (
    Shizu_State* state
  )
{
  Shizu_State_deallocateNamedMemory(state, namedMemoryName);
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
			Shizu_Gc_visitValue(state, &node->key);
			Shizu_Gc_visitValue(state, &node->value);
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

Shizu_Map*
Shizu_Map_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Shizu_Map_getType(state);
  Shizu_Map* self = (Shizu_Map*)Shizu_Gc_allocate(state, sizeof(Shizu_Map));
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
  return self;
}
