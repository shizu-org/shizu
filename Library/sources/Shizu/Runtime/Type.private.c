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
#include "Shizu/Runtime/Type.private.h"

// malloc, free
#include <malloc.h>

// strlen, strcmp
#include <string.h>

// fprintf, stderr
#include <stdio.h>

void
Shizu_Types_initialize
  ( 
    Shizu_State1* state1,
    Shizu_Types* self
  )
{
  self->elements = malloc(sizeof(Shizu_Type) * 8);
  if (!self->elements) {
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    self->elements[i] = NULL;
  }
  self->size = 0;
  self->capacity = 8;
}

void
Shizu_Types_ensureDispatchUninitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  if (0 == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  for (uint8_t i = 0, n = SmallTypeArray_getSize(&type->children); i < n; ++i) {
    Shizu_Types_ensureDispatchUninitialized(state1, self, type->children.elements[i]);
  }
  if (type->descriptor->dispatchUninitialize) {
    type->descriptor->dispatchUninitialize(state1, type->dispatch);
  }
  free(type->dispatch);
  type->dispatch = NULL;
  type->flags = ~Shizu_TypeFlags_DispatchInitialized & type->flags;
}

void
Shizu_Types_ensureDispatchInitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  if (Shizu_TypeFlags_DispatchInitialized == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  if (type->parentType) {
    if (type->descriptor->dispatchSize < type->parentType->descriptor->dispatchSize) {
      Shizu_State1_setStatus(state1, 1);
      Shizu_State1_jump(state1);
    }
    Shizu_Types_ensureDispatchInitialized(state1, self, type->parentType);
  }
  if (type->descriptor->dispatchSize < sizeof(Shizu_Object_Dispatch)) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  type->dispatch = malloc(type->descriptor->dispatchSize);
  if (!type->dispatch) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  memset(type->dispatch, 0, type->descriptor->dispatchSize);
  if (type->parentType) {
    memcpy(type->dispatch, type->parentType->dispatch, type->parentType->descriptor->dispatchSize);
  }
  if (type->descriptor->dispatchInitialize) {
    Shizu_JumpTarget jumpTarget;
    Shizu_State1_pushJumpTarget(state1, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      type->descriptor->dispatchInitialize(state1, type->dispatch);
      Shizu_State1_popJumpTarget(state1);
    } else {
      Shizu_State1_popJumpTarget(state1);
      free(type->dispatch);
      type->dispatch = NULL;
      Shizu_State1_jump(state1);
    }
  }
  type->flags |= Shizu_TypeFlags_DispatchInitialized;
}

void
Shizu_Type_destroy
  (
    Shizu_State* state,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  // Invoke the finalizer.
  if (Shizu_TypeFlags_StaticallyInitialized == (Shizu_TypeFlags_StaticallyInitialized & type->flags)) {
    if (type->descriptor->staticFinalize) {
      type->descriptor->staticFinalize(state);
    }
    type->flags = type->flags & ~Shizu_TypeFlags_StaticallyInitialized;
  }
  // Remove this type from the array of references to child types of its parent type.
  if (type->parentType) {
    SmallTypeArray_ensureRemoved(&type->parentType->children, type);
  }
  // Deallocate array of references to children.
  SmallTypeArray_uninitialize(&type->children);
  // Deallocate the name.
  free(type->name.bytes);
  type->name.bytes = NULL;
  //
  if (type->dl) {
    Shizu_Dl_unref(state, type->dl);
    type->dl = NULL;
  }
  // Deallocate the value.
  free(type);
}

size_t
Shizu_Types_getTypeChildCount
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  return SmallTypeArray_getSize(&type->children);
}

Shizu_Type*
Shizu_Types_getTypeByName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* name
  )
{
  size_t numberOfBytes = strlen(name) + 1;
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)name[i];
  }
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!strcmp(type->name.bytes, name)) {
        return type;
      }
    }
  }
  return NULL;
}

Shizu_Type*
Shizu_Types_createType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* typeDescriptor
  )
{
  size_t numberOfBytes = strlen(name) + 1;
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)name[i];
  }
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!strcmp(type->name.bytes, name)) {
        fprintf(stderr, "%s:%d: a type of name `%s` was already registered\n", __FILE__, __LINE__, name);
        Shizu_State1_setStatus(state1, 1);
        Shizu_State1_jump(state1);
      }
    }
  }
  Shizu_Type* type = malloc(sizeof(Shizu_Type));
  if (!type) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type));
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  type->name.bytes = malloc(numberOfBytes);
  if (!type->name.bytes) {
    free(type);
    type = NULL;
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  memcpy(type->name.bytes, name, numberOfBytes);
  type->flags = 0;
  type->dispatch = NULL;
  type->parentType = parentType;
  type->descriptor = typeDescriptor;
  type->name.hashValue = hashValue;
  type->name.numberOfBytes = numberOfBytes;
  type->next = NULL;
  type->typeDestroyed = typeDestroyed;
  type->dl = dl;
  // Allocate array for references to children.
  if (SmallTypeArray_initialize(&type->children)) {
    free(type->name.bytes);
    type->name.bytes = NULL;
    free(type);
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  // Add this type to the array of references to children of its parent type.
  if (parentType) {
    if (SmallTypeArray_append(&parentType->children, type)) {
      SmallTypeArray_uninitialize(&parentType->children);
      free(type->name.bytes);
      type->name.bytes = NULL;
      free(type);
      Shizu_State1_setStatus(state1, 1);
      Shizu_State1_jump(state1);
    }
  }
  type->next = self->elements[hashIndex];
  self->elements[hashIndex] = type;
  self->size++;
  return type;
}
