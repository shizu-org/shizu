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
#include "Shizu/Runtime/Objects/List.private.h"

// fprintf, stderr
#include <stdio.h>

// memmove
#include <string.h>

// malloc, realloc, free
#include <malloc.h>

#include "idlib/bit_scan.h"

#include "Shizu/Runtime/State1.h"

static void
Shizu_List_postCreateType
	(
		Shizu_State1* state1
	);

static void
Shizu_List_preDestroyType
	(
		Shizu_State1* state1
	);

static void
Shizu_List_visit
  (
    Shizu_State* state,
    Shizu_List* self
  );

static void
Shizu_List_finalize
  (
    Shizu_State* state,
    Shizu_List* self
  );

static Shizu_TypeDescriptor const Shizu_List_Type = {
	.postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_List_postCreateType,
	.preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_List_preDestroyType,
	.visitType = NULL,
	.size = sizeof(Shizu_List),
  .visit = (Shizu_OnVisitCallback*) & Shizu_List_visit,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_List_finalize,
	.dispatchSize = sizeof(Shizu_List_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

static const char* namedMemoryName = "Shizu.Lists.NamedMemory";

typedef struct Lists {
	size_t minimumCapacity;
	size_t maximumCapacity;
} Lists;

static void
Shizu_List_postCreateType
	(
		Shizu_State1* state1
	)
{
	if (Shizu_State1_allocateNamedStorage(state1, namedMemoryName, sizeof(Lists))) {
		Shizu_State1_setStatus(state1, 1);
		Shizu_State1_jump(state1);
	}
	Lists* g = NULL;
	if (Shizu_State1_getNamedStorage(state1, namedMemoryName, &g)) {
		Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
		Shizu_State1_setStatus(state1, 1);
		Shizu_State1_jump(state1);
	}
  g->minimumCapacity = 8;
  g->maximumCapacity = SIZE_MAX / sizeof(Shizu_Value);
  if (g->maximumCapacity > Shizu_Integer32_Maximum) {
    g->maximumCapacity = Shizu_Integer32_Maximum;
	}
}

static void
Shizu_List_preDestroyType
	(
		Shizu_State1* state1
	)
{
	Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
}

static void
Shizu_List_visit
  (
    Shizu_State* state,
    Shizu_List* self
  )
{
	for (size_t i = 0, n = self->size; i < n; ++i) {
		Shizu_Gc_visitValue(Shizu_State_getState1(state), Shizu_State_getGc(state), self->elements + i);
	}
}

static void
Shizu_List_finalize
  (
    Shizu_State* state,
    Shizu_List* self
  )
{
	self->size = 0;
	free(self->elements);
	self->elements = NULL;
	self->capacity = 0;
}

Shizu_defineType(Shizu_List, Shizu_Object);

Shizu_List*
Shizu_List_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Shizu_List_getType(state);
  Shizu_List* self = (Shizu_List*)Shizu_Gc_allocateObject(state, sizeof(Shizu_List));
	self->elements = malloc(8 * sizeof(Shizu_Value));
	if (!self->elements) {
		Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
	}
	self->size = 0;
	self->capacity = 8;
  ((Shizu_Object*)self)->type = TYPE;
  return self;
}

static Shizu_Value const IndexOutOfBounds = { .tag = Shizu_Value_Tag_Void, .voidValue = Shizu_Void_Void };

Shizu_Value
Shizu_List_getValue
	(
		Shizu_State* state,
		Shizu_List* self,
		size_t index
	)
{
	if (index >= self->size) {
		return IndexOutOfBounds;
	}
	return self->elements[index];
}

Shizu_Value
Shizu_List_getSize
	(
		Shizu_State* state,
		Shizu_List* self
	)
{
	Shizu_Value value;
	Shizu_Value_setInteger32(&value, (Shizu_Integer32)self->size);
	return value;
}

void
Shizu_List_insertValue
	(
		Shizu_State* state,
		Shizu_List* self,
		size_t index,
		Shizu_Value const* value
	)
{
	if (Shizu_Value_isVoid(value)) {
		return;
	}
	if (self->capacity == self->size) {
		size_t oldCapacity = self->capacity;
		size_t newCapacity;
		Lists* g = NULL;
		if (Shizu_State1_getNamedStorage(Shizu_State_getState1(state), namedMemoryName, &g)) {
			Shizu_State1_deallocateNamedStorage(Shizu_State_getState1(state), namedMemoryName);
			Shizu_State_setStatus(state, 1);
			Shizu_State_jump(state);
		}
		if (oldCapacity > g->maximumCapacity / 2) {
			// as the following fact holds
			// oldCapacity * 2 > maximumCapacity if and only if oldCapacity > maximumCapacity / 2
			// we cannot double the capacity.
			// try to saturate the capacity.
			if (oldCapacity == g->maximumCapacity) {
				Shizu_State_setStatus(state, 1);
				Shizu_State_jump(state);
			} else {
				newCapacity = g->maximumCapacity;
			}
		} else {
			newCapacity = oldCapacity * 2;
		}
		Shizu_Value* newElements = realloc(self->elements, newCapacity * sizeof(Shizu_Value));
		if (!newElements) {
			Shizu_State_setStatus(state, 1);
			Shizu_State_jump(state);
		}
		self->elements = newElements;
		self->capacity = newCapacity;
	}
	if (index < self->size) {
		memmove(self->elements + index,
						self->elements + index + 1,
						sizeof(Shizu_Value) * (self->size - index));
	}
	self->elements[index] = *value;
	self->size++;
}

void
Shizu_List_appendValue
	(
		Shizu_State* state,
		Shizu_List* self,
		Shizu_Value const* value
	)
{
	Shizu_List_insertValue(state, self, self->size, value);
}

void
Shizu_List_prependValue
	( 
		Shizu_State* state,
		Shizu_List* self,
		Shizu_Value const* value
	)
{
	Shizu_List_insertValue(state, self, 0, value);
}
