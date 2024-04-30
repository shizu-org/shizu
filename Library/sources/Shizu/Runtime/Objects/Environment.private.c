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
#include "Shizu/Runtime/Objects/Environment.private.h"

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static void
Shizu_Environment_staticInitialize
  (
    Shizu_State* state
  );

static void
Shizu_Environment_staticFinalize
  ( 
    Shizu_State* state
  );
  
static void
Shizu_Environment_visit
  (
    Shizu_State* state,
    Shizu_Environment* self
  );

static void
Shizu_Environment_finalize
  (
    Shizu_State* state,
    Shizu_Environment* self
  );

static Shizu_TypeDescriptor const Shizu_Environment_Type = {
  .staticInitialize = &Shizu_Environment_staticInitialize,
  .staticFinalize = &Shizu_Environment_staticFinalize,
  .staticVisit = NULL,
  .size = sizeof(Shizu_Environment),
  .visit = (Shizu_OnVisitCallback*) & Shizu_Environment_visit,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_Environment_finalize,
  .dispatchSize = sizeof(Shizu_Environment_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

static const char* namedMemoryName = "Shizu.Environments.NamedMemory";

typedef struct Environments {
  size_t minimumCapacity;
  size_t maximumCapacity;
} Environments;

static void
Shizu_Environment_staticInitialize
  (
    Shizu_State* state
  )
{
  if (Shizu_State1_allocateNamedStorage(Shizu_State_getState1(state), namedMemoryName, sizeof(Environments))) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Environments* g = NULL;
  if (Shizu_State1_getNamedStorage(Shizu_State_getState1(state), namedMemoryName, &g)) {
    Shizu_State1_deallocateNamedStorage(Shizu_State_getState1(state), namedMemoryName);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  g->minimumCapacity = 8;
  g->maximumCapacity = SIZE_MAX / sizeof(Shizu_Environment_Node*);
  if (g->maximumCapacity > Shizu_Integer32_Maximum) {
    g->maximumCapacity = Shizu_Integer32_Maximum;
  }
}

static void
Shizu_Environment_staticFinalize
  (
    Shizu_State* state
  )
{
  Shizu_State1_deallocateNamedStorage(Shizu_State_getState1(state), namedMemoryName);
}

static void
Shizu_Environment_visit
  (
    Shizu_State* state,
    Shizu_Environment* self
  )
{
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
		Shizu_Environment_Node* node = self->buckets[i];
		while (node) {
			Shizu_Gc_visitObject(state, (Shizu_Object*)node->key);
			Shizu_Gc_visitValue(state, &node->value);
			node = node->next;
		}
	}
}

void
Shizu_Environment_finalize
  (
		Shizu_State* state,
		Shizu_Environment* self
	)
{
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
		Shizu_Environment_Node** bucket = &(self->buckets[i]);
		while (*bucket) {
			Shizu_Environment_Node* node = *bucket;
			*bucket = node->next;
			free(node);
		}
	}
	free(self->buckets);
  self->buckets = NULL;
}

Shizu_defineType(Shizu_Environment, Shizu_Object);

Shizu_Environment*
Shizu_Environment_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Shizu_Environment_getType(state);
  Shizu_Environment* self = (Shizu_Environment*)Shizu_Gc_allocate(state, sizeof(Shizu_Environment));
  self->buckets = malloc(sizeof(Shizu_Environment_Node*) * 8);
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

Shizu_Integer32
Shizu_Environment_getSize
  (
    Shizu_State* state,
    Shizu_Environment* self
  )
{
return self->size;
}

void
Shizu_Environment_define
  (
    Shizu_State* state,
    Shizu_Environment* self,
    Shizu_String* key
  )
{ }

Shizu_Boolean
Shizu_Environment_ísDefined
  (
    Shizu_State* state,
    Shizu_Environment* self,
    Shizu_String* key
  )
{  
  return Shizu_Boolean_False;
}