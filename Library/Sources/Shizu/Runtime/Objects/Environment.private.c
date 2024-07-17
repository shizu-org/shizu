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

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"

#include "Shizu/Runtime/Objects/CxxProcedure.h"
#include "Shizu/Runtime/Objects/List.h"
#include "Shizu/Runtime/Objects/Map.h"
#include "Shizu/Runtime/Objects/String.h"
#include "Shizu/Runtime/Objects/WeakReference.h"

// memcmp, memcpy
#include <string.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "idlib/bit_scan.h"

static void
Shizu_Environment_postCreateType
  (
    Shizu_State1* state1
  );

static void
Shizu_Environment_preDestroyType
  (
    Shizu_State1* state1
  );

static void
Shizu_Environment_visit
  (
    Shizu_State2* state,
    Shizu_Environment* self
  );

static void
Shizu_Environment_finalize
  (
    Shizu_State2* state,
    Shizu_Environment* self
  );

static void
Shizu_Environment_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Shizu_Environment_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) & Shizu_Environment_postCreateType,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) & Shizu_Environment_preDestroyType,
  .visitType = NULL,
  .size = sizeof(Shizu_Environment),
  .construct = &Shizu_Environment_constructImpl,
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
Shizu_Environment_postCreateType
  (
    Shizu_State1* state1
  )
{
  if (Shizu_State1_allocateNamedStorage(state1, namedMemoryName, sizeof(Environments))) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  Environments* g = NULL;
  if (Shizu_State1_getNamedStorage(state1, namedMemoryName, (void**)&g)) {
    Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
  }
  g->minimumCapacity = 8;
  g->maximumCapacity = SIZE_MAX / sizeof(Shizu_Environment_Node*);
  if (g->maximumCapacity > Shizu_Integer32_Maximum) {
    g->maximumCapacity = Shizu_Integer32_Maximum;
  }
}

static void
Shizu_Environment_preDestroyType
  (
    Shizu_State1* state1
  )
{
  Shizu_State1_deallocateNamedStorage(state1, namedMemoryName);
}

static void
Shizu_Environment_visit
  (
    Shizu_State2* state,
    Shizu_Environment* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_Environment_Node* node = self->buckets[i];
    while (node) {
      Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)node->name);
      Shizu_Gc_visitValue(Shizu_State2_getState1(state), Shizu_State2_getGc(state), &node->value);
      node = node->next;
    }
  }
}

static void
Shizu_Environment_finalize
  (
    Shizu_State2* state,
    Shizu_Environment* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Shizu_Environment_Node** bucket = &(self->buckets[i]);
    while (*bucket) {
      Shizu_Environment_Node* node = *bucket;
      *bucket = node->next;
      Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
    }
  }
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self->buckets);
  self->buckets = NULL;
}

static void
Shizu_Environment_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isObject(&argumentValues[0])) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Environment* SELF = (Shizu_Environment*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Type* TYPE = Shizu_Environment_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->buckets = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Shizu_Environment_Node*) * 8);
  if (!SELF->buckets) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    SELF->buckets[i] = NULL;
  }
  SELF->size = 0;
  SELF->capacity = 8;
  ((Shizu_Object*)SELF)->type = TYPE;
}

Shizu_defineObjectType("Shizu.Environment", Shizu_Environment, Shizu_Object);

Shizu_Integer32
Shizu_Environment_getSize
  (
    Shizu_State2* state,
    Shizu_Environment* self
  )
{
  return self->size;
}

void
Shizu_Environment_set
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Value* value
  )
{
  size_t hashValue = Shizu_Object_getHashValue(state, (Shizu_Object*)name);
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Environment_Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)node->name, (Shizu_Object*)name)) {
      node->name = name;
      node->value = *value;
      return;
    }
  }
  Shizu_Environment_Node* node = malloc(sizeof(Shizu_Environment_Node));
  if (!node) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  node->name = name;
  node->value = *value;
  node->next = self->buckets[hashIndex];
  self->buckets[hashIndex] = node;
  self->size++;
}

Shizu_Value
Shizu_Environment_get
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* key
  )
{
  size_t hashValue = Shizu_Object_getHashValue(state, (Shizu_Object*)key);
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Environment_Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)node->name, (Shizu_Object*)key)) {
      return node->value;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_NotExists);
  Shizu_State2_jump(state);
}

Shizu_Object*
Shizu_Environment_getObject
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Type* type
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != name);
  Shizu_debugAssert(NULL != type);
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), type, Shizu_Object_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value value = Shizu_Environment_get(state, self, name);
  if (!Shizu_Value_isObject(&value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&value)->type, type)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getObject(&value);
}

Shizu_Boolean
Shizu_Environment_getBoolean
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Value value = Shizu_Environment_get(state, self, name);
  if (!Shizu_Value_isBoolean(&value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getBoolean(&value);
}

Shizu_CxxProcedure*
Shizu_Environment_getCxxProcedure
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_CxxProcedure_getType(state);
  return (Shizu_CxxProcedure*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_Environment*
Shizu_Environment_getEnvironment
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_Environment_getType(state);
  return (Shizu_Environment*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_Integer32
Shizu_Environment_getInteger32
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Value value = Shizu_Environment_get(state, self, name);
  if (!Shizu_Value_isInteger32(&value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getInteger32(&value);
}

Shizu_List*
Shizu_Environment_getList
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_List_getType(state);
  return (Shizu_List*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_Map*
Shizu_Environment_getMap
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_Map_getType(state);
  return (Shizu_Map*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_String*
Shizu_Environment_getString
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_String_getType(state);
  return (Shizu_String*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_WeakReference*
Shizu_Environment_getWeakReference
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  )
{
  Shizu_Type* type = Shizu_WeakReference_getType(state);
  return (Shizu_WeakReference*)Shizu_Environment_getObject(state, self, name, type);
}

Shizu_Boolean
Shizu_Environment_isDefined
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* key
  )
{
  size_t hashValue = Shizu_Object_getHashValue(state, (Shizu_Object*)key);
  size_t hashIndex = hashValue % self->capacity;
  for (Shizu_Environment_Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)node->name, (Shizu_Object*)key)) {
      Shizu_Boolean_True;
    }
  }
  return Shizu_Boolean_False;
}
