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
#include "Shizu/Runtime/Stack.private.h"

// malloc, free
#include <malloc.h>

struct Shizu_Stack {
  Shizu_Value* elements;
  size_t size;
  size_t capacity;
  size_t minimalCapacity;
  size_t maximalCapacity;
};

Shizu_Stack*
Shizu_Stack_startup
  (
    Shizu_State* state
  )
{
  Shizu_Stack* self = malloc(sizeof(Shizu_Stack));
  if (!self) {
    Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State_jump(state);
  }

  // (2)
  self->minimalCapacity = 8;
  self->maximalCapacity = SIZE_MAX / sizeof(Shizu_Value);
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
  self->elements = malloc(sizeof(Shizu_Value) * self->capacity);
  if (!self->elements) {
    free(self);
    self = NULL;
    Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State_jump(state);
  }
  self->size = 0;
  return self;
}

void
Shizu_Stack_shutdown
  (
    Shizu_State* state,
    Shizu_Stack* self
  )
{
  self->size = 0;
  
  free(self->elements);
  self->elements = NULL;
  self->capacity = 0;
  
  free(self);
  self = NULL;
}

void
Shizu_Stack_notifyPreMark
  (
    Shizu_State* state
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  for (size_t i = 0, n = stack->size; i < n; ++i) {
    Shizu_Gc_visitValue(state, stack->elements + i);
  }
}

size_t
Shizu_Stack_getSize
  (
    Shizu_State* state
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  return stack->size;
}

Shizu_Value
Shizu_Stack_peek
  (
    Shizu_State* state
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (0 == stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return *(stack->elements + (stack->size - 1));
}

void
Shizu_Stack_pop
  (
    Shizu_State* state
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (0 == stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  --stack->size;
}

void
Shizu_Stack_push
  (
    Shizu_State* state,
    Shizu_Value value
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (stack->capacity == stack->size) {
    if (stack->maximalCapacity == stack->capacity) {
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    }
    size_t newCapacity = stack->capacity * 2;
    if (newCapacity <= stack->capacity) {
      newCapacity = stack->maximalCapacity;
    }
    Shizu_Value* newElements = realloc(stack->elements, sizeof(Shizu_Value) * newCapacity);
    if (!newElements) {
      Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State_jump(state);
    }
    stack->capacity = newCapacity;
    stack->elements = newElements;
  }
  stack->elements[stack->size++] = value;
}

bool
Shizu_Stack_isBoolean
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isBoolean(stack->elements + stack->size - index - 1);
}

bool
Shizu_Stack_isCxxFunction
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isCxxFunction(stack->elements + stack->size - index - 1);
}

bool
Shizu_Stack_isFloat32
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isFloat32(stack->elements + stack->size - index - 1);
}

bool
Shizu_Stack_isInteger32
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isInteger32(stack->elements + stack->size - index - 1);
}

bool
Shizu_Stack_isObject
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isObject(stack->elements + stack->size - index - 1);
}

bool
Shizu_Stack_isVoid
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_isVoid(stack->elements + stack->size - index - 1);
}

Shizu_Boolean
Shizu_Stack_getBoolean
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isBoolean(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getBoolean(value);
}

Shizu_Reference(Shizu_CxxFunction)
Shizu_Stack_getCxxFunction
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isCxxFunction(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getCxxFunction(value);
}

Shizu_Float32
Shizu_Stack_getFloat32
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isFloat32(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getFloat32(value);
}

Shizu_Integer32
Shizu_Stack_getInteger32
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isInteger32(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getInteger32(value);
}

Shizu_Reference(Shizu_Object)
Shizu_Stack_getObject
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isObject(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getObject(value);
}

Shizu_Void
Shizu_Stack_getVoid
  (
    Shizu_State* state,
    size_t index
  )
{
  Shizu_Stack* stack = Shizu_State_getStack(state);
  if (index >= stack->size) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Value* value = stack->elements + stack->size - index - 1;
  if (!Shizu_Value_isVoid(value)) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getVoid(value);
}
