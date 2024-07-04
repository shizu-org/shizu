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

#include "DataDefinitionLanguage/Atoms.h"

// memcpy
#include <string.h>

static void
Atom_finalize
  (
    Shizu_State2* state,
    Atom* self
  );

static void
Atom_visit
  (
    Shizu_State2* state,
    Atom* self
  );

static Shizu_ObjectTypeDescriptor const Atom_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Atom),
  .visit = (Shizu_OnVisitCallback*)&Atom_visit,
  .finalize = (Shizu_OnFinalizeCallback*)&Atom_finalize,
  .dispatchSize = sizeof(Atom_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Atom, Shizu_Object);

static void
Atom_finalize
  (
    Shizu_State2* state,
    Atom* self
  )
{
  if (self->bytes) {
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self->bytes);
    self->bytes = NULL;
  }
}

static void
Atom_visit
  (
    Shizu_State2* state,
    Atom* self
  )
{
  if (self->next) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->next);

  }
}

void
Atom_construct
  (
    Shizu_State2* state,
    Atom* self,
    Shizu_ByteArray* bytes
  )
{
  Shizu_Type* TYPE = Atom_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  size_t numberOfRawBytes = Shizu_ByteArray_getNumberOfRawBytes(state, bytes);
  uint8_t const* rawBytes = Shizu_ByteArray_getRawBytes(state, bytes);
  size_t hashValue = numberOfRawBytes;
  for (size_t i = 0, n = numberOfRawBytes; i < n; ++i) {
    hashValue = (hashValue << 5)^(hashValue >> 3) | (size_t)rawBytes[i];
  }
  self->bytes = Shizu_State1_allocate(Shizu_State2_getState1(state), numberOfRawBytes);
  if (!self->bytes) {
    Shizu_State1_setStatus(Shizu_State2_getState1(state), Shizu_Status_AllocationFailed);
    Shizu_State1_jump(Shizu_State2_getState1(state));
  }
  memcpy(self->bytes, rawBytes, numberOfRawBytes);
  self->numberOfBytes = numberOfRawBytes;
  self->hashValue = hashValue;
  self->next = NULL;
  ((Shizu_Object*)self)->type = TYPE;
}

Atom*
Atom_create
  (
    Shizu_State2* state,
    Shizu_ByteArray* bytes
  )
{
  Shizu_Type* TYPE = Atom_getType(state);
  Atom* self = (Atom*)Shizu_Gc_allocateObject(state, sizeof(Atom));
  Atom_construct(state, self, bytes);
  return self;
}

static void
Atoms_finalize
  (
    Shizu_State2* state,
    Atoms* self
  );

static void
Atoms_visit
  (
    Shizu_State2* state,
    Atoms* self
  );

static Shizu_ObjectTypeDescriptor const Atoms_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Atoms),
  .visit = (Shizu_OnVisitCallback*)&Atoms_visit,
  .finalize = (Shizu_OnFinalizeCallback*)&Atoms_finalize,
  .dispatchSize = sizeof(Atoms_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Atoms, Shizu_Object);

static void
Atoms_finalize
  (
    Shizu_State2* state,
    Atoms* self
  )
{
  if (self->buckets) {
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self->buckets);
    self->buckets = NULL;
  }
}

static void
Atoms_visit
  (
    Shizu_State2* state,
    Atoms* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Atom* node  = self->buckets[i];
    while (node) {
      Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)node);
      node = node->next;
    }
  }
}

void
Atoms_construct
  (
    Shizu_State2* state,
    Atoms* self
  )
{
  Shizu_Type* TYPE = Atoms_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->buckets = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Atom*) * 8);
  if (!self->buckets) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  self->size = 0;
  self->capacity = 8;
  ((Shizu_Object*)self)->type = TYPE;
}

Atoms*
Atoms_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Atoms_getType(state);
  Atoms* self = (Atoms*)Shizu_Gc_allocateObject(state, sizeof(Atoms));
  Atoms_construct(state, self);
  return self;
}
