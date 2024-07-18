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

// strlen, strcmp
#include <string.h>

// fprintf, stderr
#include <stdio.h>

#include "Shizu/Runtime/Object.h"

Shizu_Types*
Shizu_Types_startup
  (
    Shizu_State1* state1
  )
{
  Shizu_Types* self = Shizu_State1_allocate(state1, sizeof(Shizu_Types));
  if (!self) {
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Types_initialize(state1, self);
    Shizu_State1_popJumpTarget(state1);
  } else {
    Shizu_State1_popJumpTarget(state1);
    Shizu_State1_deallocate(state1, self);
    Shizu_State1_jump(state1);
  }
  return self;
}

void
Shizu_Types_shutdown
  (
    Shizu_State1* state1,
    Shizu_Types* self
  )
{
  Shizu_Types_uninitialize(state1, self);
  Shizu_State1_deallocate(state1, self);
}

void
Shizu_Types_initialize
  (
    Shizu_State1* state1,
    Shizu_Types* self
  )
{
  self->elements = Shizu_State1_allocate(state1, sizeof(Shizu_Type) * 8);
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
Shizu_Types_uninitialize
  (
    Shizu_State1* state1,
    Shizu_Types* self
  )
{
  // Uninitialize all dispatches such that
  // the dispatch of a child type is uninitialized before the dispatch of its parent type.
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_Type* type = self->elements[i];
    while (type) {
      Shizu_Types_ensureDispatchUninitialized(state1, self, type);
      type = type->next;
    }
  }
  while (self->size) {
    // Worst-case is k^n where n is the number of types and k is the capacity.
    // Do *no*u use this in performance critical code.
    for (size_t i = 0, n = self->capacity; i < n; ++i) {
      Shizu_Type** previous = &(self->elements[i]);
      Shizu_Type* current = self->elements[i];
      while (current) {
        if (!Shizu_Types_getTypeChildCount(state1, self, current)) {
          if (0 != (Shizu_TypeFlags_DispatchInitialized & current->flags)) {
            fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
          }
          Shizu_Type* type = current;
          *previous = current->next;
          current = current->next;
          self->size--;
          if (type->typeDestroyed) {
            type->typeDestroyed(state1);
          }
          Shizu_Type_destroy(state1, self, type);
        } else {
          previous = &current->next;
          current = current->next;
        }
      }
    }
  }
}

void
Shizu_Types_onPostCreateType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    if (Shizu_TypeFlags_ObjectType == (Shizu_TypeFlags_ObjectType & type->flags)) {
      if (type->objectType.descriptor->postCreateType) {
        type->objectType.descriptor->postCreateType(state1);
      }
    } else {
      if (type->primitiveType.descriptor->postCreateType) {
        type->primitiveType.descriptor->postCreateType(state1);
      }
    }
    type->flags |= Shizu_TypeFlags_PostTypeCreationInvoked;
    Shizu_Types_ensureDispatchInitialized(state1, self, type);
  }
  Shizu_State1_popJumpTarget(state1);
}

void
Shizu_Types_onPreDestroyType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  if (Shizu_TypeFlags_PostTypeCreationInvoked == (Shizu_TypeFlags_PostTypeCreationInvoked & type->flags)) {
    if (Shizu_TypeFlags_ObjectType == (Shizu_TypeFlags_ObjectType & type->flags)) {
      if (type->objectType.descriptor->preDestroyType) {
        type->objectType.descriptor->preDestroyType(state1);
      }
    } else {
      if (type->primitiveType.descriptor->preDestroyType) {
        type->primitiveType.descriptor->preDestroyType(state1);
      }
    }
    type->flags = type->flags & ~Shizu_TypeFlags_PostTypeCreationInvoked;
  }
}

void
Shizu_Types_ensureDispatchUninitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & type->flags)) {
    return;
  }
  if (0 == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  for (uint8_t i = 0, n = SmallTypeArray_getSize(&type->objectType.children); i < n; ++i) {
    Shizu_Types_ensureDispatchUninitialized(state1, self, type->objectType.children.elements[i]);
  }
  if (type->objectType.descriptor->dispatchUninitialize) {
    type->objectType.descriptor->dispatchUninitialize(state1, type->objectType.dispatch);
  }
  Shizu_State1_deallocate(state1, type->objectType.dispatch);
  type->objectType.dispatch = NULL;
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
  if (0 == (Shizu_TypeFlags_ObjectType & type->flags)) {
    return;
  }
  if (Shizu_TypeFlags_DispatchInitialized == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  if (type->objectType.parentType) {
    if (type->objectType.descriptor->dispatchSize < type->objectType.parentType->objectType.descriptor->dispatchSize) {
      Shizu_State1_setStatus(state1, 1);
      Shizu_State1_jump(state1);
    }
    Shizu_Types_ensureDispatchInitialized(state1, self, type->objectType.parentType);
  }
  if (type->objectType.descriptor->dispatchSize < sizeof(Shizu_Object_Dispatch)) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  type->objectType.dispatch = Shizu_State1_allocate(state1, type->objectType.descriptor->dispatchSize);
  if (!type->objectType.dispatch) {
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  memset(type->objectType.dispatch, 0, type->objectType.descriptor->dispatchSize);
  if (type->objectType.parentType) {
    memcpy(type->objectType.dispatch, type->objectType.parentType->objectType.dispatch, type->objectType.parentType->objectType.descriptor->dispatchSize);
  }
  if (type->objectType.descriptor->dispatchInitialize) {
    Shizu_JumpTarget jumpTarget;
    Shizu_State1_pushJumpTarget(state1, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      type->objectType.descriptor->dispatchInitialize(state1, type->objectType.dispatch);
      Shizu_State1_popJumpTarget(state1);
    } else {
      Shizu_State1_popJumpTarget(state1);
      Shizu_State1_deallocate(state1, type->objectType.dispatch);
      type->objectType.dispatch = NULL;
      Shizu_State1_jump(state1);
    }
  }
  type->flags |= Shizu_TypeFlags_DispatchInitialized;
}

void
Shizu_Type_destroy
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  Shizu_Types_onPreDestroyType(state1, self, type);
  if (Shizu_TypeFlags_ObjectType == (Shizu_TypeFlags_ObjectType & type->flags)) {
    // Remove this type from the array of references to child types of its parent type.
    if (type->objectType.parentType) {
      SmallTypeArray_ensureRemoved(&type->objectType.parentType->objectType.children, type);
    }
    // Deallocate array of references to children.
    SmallTypeArray_uninitialize(&type->objectType.children);
  }
  // Deallocate the name.
  Shizu_State1_deallocate(state1, type->name.bytes);
  type->name.bytes = NULL;
  //
  if (type->dl) {
    Shizu_State1_unrefDl(state1, type->dl);
    type->dl = NULL;
  }
  // Deallocate the value.
  Shizu_State1_deallocate(state1, type);
}

size_t
Shizu_Types_getTypeChildCount
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  )
{
  if (0 == (Shizu_TypeFlags_ObjectType & type->flags)) {
    return 0;
  }
  return SmallTypeArray_getSize(&type->objectType.children);
}

Shizu_Type*
Shizu_Types_getTypeByName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    uint8_t const* bytes,
    Shizu_Integer32 numberOfBytes
  )
{
  Shizu_Integer32 hashValue = numberOfBytes;
  for (Shizu_Integer32 i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (Shizu_Integer32)bytes[i];
  }
  Shizu_Integer32 hashIndex = (hashValue & (0x7FFFFFFF)) % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!memcmp(type->name.bytes, bytes, (size_t)numberOfBytes)) {
        return type;
      }
    }
  }
  return NULL;
}

Shizu_Type*
Shizu_Types_createObjectType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_ObjectTypeDescriptor const* typeDescriptor
  )
{
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)bytes[i];
  }
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!memcmp(type->name.bytes, bytes, numberOfBytes)) {
        fprintf(stderr, "%s:%d: a type of name `%.*s` was already registered\n", __FILE__, __LINE__, (int)numberOfBytes, bytes);
        /// @todo Add and use Shizu_Status_TypeExists.
        Shizu_State1_setStatus(state1, 1/*Shizu_Status_TypeExists*/);
        Shizu_State1_jump(state1);
      }
    }
  }
  Shizu_Type* type = Shizu_State1_allocate(state1, sizeof(Shizu_Type));
  if (!type) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type));
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  type->name.bytes = Shizu_State1_allocate(state1, numberOfBytes);
  if (!type->name.bytes) {
    Shizu_State1_deallocate(state1, type);
    type = NULL;
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  memcpy(type->name.bytes, bytes, numberOfBytes);
  type->name.hashValue = hashValue;
  type->name.numberOfBytes = numberOfBytes;
  type->next = NULL;
  type->flags = Shizu_TypeFlags_ObjectType;
  type->typeDestroyed = typeDestroyed;
  type->dl = dl;
  if (type->dl) {
    Shizu_State1_refDl(state1, type->dl);
  }

  type->objectType.dispatch = NULL;
  type->objectType.parentType = parentType;
  type->objectType.descriptor = typeDescriptor;
  // Allocate array for references to children.
  if (SmallTypeArray_initialize(&type->objectType.children)) {
    Shizu_State1_deallocate(state1, type->name.bytes);
    type->name.bytes = NULL;
    Shizu_State1_deallocate(state1, type);
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  // Add this type to the array of references to children of its parent type.
  if (parentType) {
    if (SmallTypeArray_append(&parentType->objectType.children, type)) {
      SmallTypeArray_uninitialize(&parentType->objectType.children);
      Shizu_State1_deallocate(state1, type->name.bytes);
      type->name.bytes = NULL;
      Shizu_State1_deallocate(state1, type);
      Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state1);
    }
  }
  type->next = self->elements[hashIndex];
  self->elements[hashIndex] = type;
  self->size++;

  Shizu_Types_onPostCreateType(state1, self, type);

  return type;
}

Shizu_Type*
Shizu_Types_createEnumerationType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_EnumerationTypeDescriptor const* typeDescriptor
  )
{
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)bytes[i];
  }
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!memcmp(type->name.bytes, bytes, numberOfBytes)) {
        fprintf(stderr, "%s:%d: a type of name `%.*s` was already registered\n", __FILE__, __LINE__, (int)numberOfBytes, bytes);
        /// @todo Add and use Shizu_Status_TypeExists.
        Shizu_State1_setStatus(state1, 1/*Shizu_Status_TypeExists*/);
        Shizu_State1_jump(state1);
      }
    }
  }
  Shizu_Type* type = Shizu_State1_allocate(state1, sizeof(Shizu_Type));
  if (!type) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type));
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  type->name.bytes = Shizu_State1_allocate(state1, numberOfBytes);
  if (!type->name.bytes) {
    Shizu_State1_deallocate(state1, type);
    type = NULL;
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  memcpy(type->name.bytes, bytes, numberOfBytes);
  type->name.hashValue = hashValue;
  type->name.numberOfBytes = numberOfBytes;
  type->next = NULL;
  type->flags = Shizu_TypeFlags_EnumerationType;
  type->typeDestroyed = typeDestroyed;
  type->dl = dl;
  if (type->dl) {
    Shizu_State1_refDl(state1, type->dl);
  }

  type->enumerationType.descriptor = typeDescriptor;

  type->next = self->elements[hashIndex];
  self->elements[hashIndex] = type;
  self->size++;

  Shizu_Types_onPostCreateType(state1, self, type);

  return type;
}

Shizu_Type*
Shizu_Types_createPrimitiveType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_PrimitiveTypeDescriptor const* typeDescriptor
  )
{
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)bytes[i];
  }
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Type* type = self->elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!memcmp(type->name.bytes, bytes, numberOfBytes)) {
        fprintf(stderr, "%s:%d: a type of name `%.*s` was already registered\n", __FILE__, __LINE__, (int)numberOfBytes, bytes);
        /// @todo Add and use Shizu_Status_TypeExists.
        Shizu_State1_setStatus(state1, 1/*Shizu_Status_TypeExists*/);
        Shizu_State1_jump(state1);
      }
    }
  }
  Shizu_Type* type = Shizu_State1_allocate(state1, sizeof(Shizu_Type));
  if (!type) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type));
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  type->name.bytes = Shizu_State1_allocate(state1, numberOfBytes);
  if (!type->name.bytes) {
    Shizu_State1_deallocate(state1, type);
    type = NULL;
    Shizu_State1_setStatus(state1, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state1);
  }
  memcpy(type->name.bytes, bytes, numberOfBytes);
  type->name.hashValue = hashValue;
  type->name.numberOfBytes = numberOfBytes;
  type->next = NULL;
  type->flags = Shizu_TypeFlags_PrimitiveType;
  type->typeDestroyed = typeDestroyed;
  type->dl = dl;
  if (type->dl) {
    Shizu_State1_refDl(state1, type->dl);
  }

  type->primitiveType.descriptor = typeDescriptor;

  type->next = self->elements[hashIndex];
  self->elements[hashIndex] = type;
  self->size++;

  Shizu_Types_onPostCreateType(state1, self, type);

  return type;
}
