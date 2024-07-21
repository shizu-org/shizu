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
#include "Shizu/Runtime/Stack.private.h"

#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/CxxUtilities.h"
#include "Shizu/Runtime/Gc.h"

struct Shizu_Stack {
  Shizu_Value* elements;
  size_t size;
  size_t capacity;
  size_t minimalCapacity;
  size_t maximalCapacity;
};

Shizu_Stack*
Shizu_Stack_create
  (
    Shizu_State1* state
  )
{
  Shizu_Stack* self = Shizu_State1_allocate(state, sizeof(Shizu_Stack));
  if (!self) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }

  // (2)
  self->minimalCapacity = 8;
  self->maximalCapacity = SIZE_MAX / sizeof(Shizu_Value);
  if (self->maximalCapacity > Shizu_Integer32_Maximum) {
    self->maximalCapacity = Shizu_Integer32_Maximum;
  }
  if (self->maximalCapacity < self->minimalCapacity) {
    Shizu_State1_deallocate(state, self);
    self = NULL;
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }

  // (3)
  self->capacity = self->minimalCapacity;
  self->elements = Shizu_State1_allocate(state, sizeof(Shizu_Value) * self->capacity);
  if (!self->elements) {
    Shizu_State1_deallocate(state, self);
    self = NULL;
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  self->size = 0;
  return self;
}

void
Shizu_Stack_destroy
  (
    Shizu_State1* state,
    Shizu_Stack* self
  )
{
  self->size = 0;

  Shizu_State1_deallocate(state, self->elements);
  self->elements = NULL;
  self->capacity = 0;

  Shizu_State1_deallocate(state, self);
  self = NULL;
}

void
Shizu_Stack_notifyPreMark
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Stack* self
  )
{
  for (size_t i = 0, n = self->size; i < n; ++i) {
    Shizu_Gc_visitValue(state1, gc, self->elements + i);
  }
}

size_t
Shizu_Stack_getSize
  (
    Shizu_State1* state,
    Shizu_Stack* self
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  return self->size;
}

Shizu_Value
Shizu_Stack_peek
  (
    Shizu_State1* state,
    Shizu_Stack* self
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (0 == self->size) {
    Shizu_State1_setStatus(state, 1);
    Shizu_State1_jump(state);
  }
  return *(self->elements + (self->size - 1));
}

void
Shizu_Stack_pop
  (
    Shizu_State1* state,
    Shizu_Stack* self
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (0 == self->size) {
    Shizu_State1_setStatus(state, 1);
    Shizu_State1_jump(state);
  }
  --self->size;
}

void
Shizu_Stack_push
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Value value
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (self->capacity == self->size) {
    if (self->maximalCapacity == self->capacity) {
      Shizu_State1_setStatus(state, Shizu_Status_StackOverflow);
      Shizu_State1_jump(state);
    }
    size_t newCapacity = self->capacity * 2;
    if (newCapacity <= self->capacity) {
      newCapacity = self->maximalCapacity;
    }
    Shizu_Value* newElements = Shizu_State1_reallocate(state, self->elements, sizeof(Shizu_Value) * newCapacity);
    if (!newElements) {
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    self->capacity = newCapacity;
    self->elements = newElements;
  }
  self->elements[self->size++] = value;
}

void
Shizu_Stack_clear
  (
    Shizu_State1* state,
    Shizu_Stack* self
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  self->size = 0;
}

bool
Shizu_Stack_isBoolean
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isBoolean(self->elements + self->size - index - 1);
}

bool
Shizu_Stack_isCxxFunction
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isCxxFunction(self->elements + self->size - index - 1);
}

bool
Shizu_Stack_isFloat32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isFloat32(self->elements + self->size - index - 1);
}

bool
Shizu_Stack_isInteger32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isInteger32(self->elements + self->size - index - 1);
}

bool
Shizu_Stack_isObject
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isObject(self->elements + self->size - index - 1);
}

bool
Shizu_Stack_isVoid
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_isVoid(self->elements + self->size - index - 1);
}

Shizu_Boolean
Shizu_Stack_getBoolean
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isBoolean(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getBoolean(value);
}

Shizu_CxxFunction*
Shizu_Stack_getCxxFunction
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isCxxFunction(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getCxxFunction(value);
}

Shizu_Float32
Shizu_Stack_getFloat32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isFloat32(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getFloat32(value);
}

Shizu_Integer32
Shizu_Stack_getInteger32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isInteger32(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getInteger32(value);
}

Shizu_Object*
Shizu_Stack_getObject
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isObject(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getObject(value);
}

Shizu_Void
Shizu_Stack_getVoid
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  )
{
  Shizu_debugAssert(NULL != state && NULL != self);
  if (index >= self->size) {
    Shizu_State1_setStatus(state, Shizu_Status_StackIndexOutOfBounds);
    Shizu_State1_jump(state);
  }
  Shizu_Value* value = self->elements + self->size - index - 1;
  if (!Shizu_Value_isVoid(value)) {
    Shizu_State1_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State1_jump(state);
  }
  return Shizu_Value_getVoid(value);
}
