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
#include "Shizu/Runtime/Locks.private.h"

#include "Shizu/Runtime/Gc.private.h"

// malloc, free
#include <malloc.h>

/// @unmanaged
typedef struct LockNode LockNode;

/// @unmanaged
typedef struct Locks Locks;

struct LockNode {
  LockNode* next;
  size_t hashValue;
  size_t count;
  Shizu_Object* object;
};

struct Shizu_Locks {
  LockNode* free;
  LockNode** buckets;
  size_t size;
  size_t capacity;
  size_t minimalCapacity;
  size_t maximalCapacity;
};

static Locks* g = NULL;

void
Shizu_Object_lock
  (
    Shizu_State* state,
    Shizu_Object* self
  )
{
  Shizu_Locks* locks = Shizu_State_getLocks(state);
  size_t hashValue = (size_t)(uintptr_t)self;
  size_t hashIndex = hashValue % locks->capacity;
  LockNode* current = locks->buckets[hashIndex];
  // Find the lock node if it exists.
  while (current != NULL && current->object != self) {
    current = current->next;
  }
  if (!current) {
    // Create the lock node if it does not exist.
    current = malloc(sizeof(LockNode));
    if (!current) {
      Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State_jump(state);
    }
    current->next = locks->buckets[hashIndex];
    locks->buckets[hashIndex] = current;
    current->hashValue = hashValue;
    current->object = self;
    current->count = 0;
  }
  // Assert the number of locks does not overflow.
  if (current->count == Shizu_Integer32_Maximum) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  // Increment the lock count.
  current->count++;
}

void
Shizu_Object_unlock
  (
    Shizu_State* state,
    Shizu_Object* self
  )
{
  Shizu_Locks* locks = Shizu_State_getLocks(state);
  size_t hashValue = (size_t)(uintptr_t)self;
  size_t hashIndex = hashValue % locks->capacity;
  LockNode* current = locks->buckets[hashIndex];
  while (current != NULL && current->object != self) {
    current = current->next;
  }
  if (!current) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (current->count == 0) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  current->count--;
}

Shizu_Locks*
Shizu_Locks_startup
  (
    Shizu_State* state
  )
{
  Shizu_Locks* self = malloc(sizeof(Shizu_Locks));
  if (!self) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  
  // (2)
  self->minimalCapacity = 8;
  self->maximalCapacity = SIZE_MAX / sizeof(LockNode*);
	if (self->maximalCapacity > Shizu_Integer32_Maximum) {
    self->maximalCapacity = Shizu_Integer32_Maximum;
	}
  if (self->maximalCapacity < self->minimalCapacity) {
    free(self);
    self = NULL;
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }

  // (3)
  self->capacity = self->minimalCapacity;
  self->buckets = malloc(sizeof(LockNode*) * self->capacity);
	if (!self->buckets) {
		free(self);
    self = NULL;
		Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State_jump(state);
	}
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
    self->buckets[i] = NULL;
	}

  // (4)
  self->size = 0;

  return self;
}

void
Shizu_Locks_shutdown
  (
    Shizu_State* state,
    Shizu_Locks* self
  )
{
  // TODO: In debug mode, assert the table must be empty.
  // (4), (3)
  self->size = 0;
	for (size_t i = 0, n = self->capacity; i < n; ++i) {
		LockNode** bucket = &(self->buckets[i]);
		while (*bucket) {
			LockNode* node = *bucket;
			*bucket = (*bucket)->next;
			free(node);
		}
	}
  self->capacity = 0;
  free(self->buckets);
  self->buckets = NULL;

  // (2)
  free(self);
  self = NULL;
}

void
Shizu_Locks_notifyPreMark
  (
    Shizu_State* state
  )
{
  Shizu_Locks* locks = Shizu_State_getLocks(state);
  // Premark locks.
  for (size_t i = 0, n = locks->capacity; i < n; ++i) {
    LockNode* lockNode = locks->buckets[i];
    while (lockNode) {
      if (lockNode->count > 0) {
        Shizu_Object* object = (Shizu_Object*)lockNode->object;
        Shizu_Gc_visitObject(state, object);
      }
      lockNode = lockNode->next;
    }
  }
}

void
Shizu_Locks_notifyDestroy
  (
    Shizu_State* state,
    Shizu_Object* object
  )
{
  Shizu_Locks* locks = Shizu_State_getLocks(state);
  size_t hashValue = (size_t)(uintptr_t)object;
  size_t hashIndex = hashValue % locks->capacity;
  LockNode** previous = &(locks->buckets[hashIndex]);
  LockNode* current = locks->buckets[hashIndex];
  // Find the lock node if it exists.
  while (current != NULL && current->object != object) {
    previous = &current->next;
    current = current->next;
  }
  if (current) {
    *previous = current->next;
    free(current);
  }
}
