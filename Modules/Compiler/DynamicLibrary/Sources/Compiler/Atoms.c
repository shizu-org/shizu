#include "Compiler/Atoms.h"

// memcpy
#include <string.h>

static void
Compiler_Atom_finalize
  (
    Shizu_State2* state,
    Compiler_Atom* self
  );

static void
Compiler_Atom_visit
  (
    Shizu_State2* state,
    Compiler_Atom* self
  );

static Shizu_ObjectTypeDescriptor const Compiler_Atom_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Atom),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Atom_visit,
  .finalize = (Shizu_OnFinalizeCallback*)&Compiler_Atom_finalize,
  .dispatchSize = sizeof(Compiler_Atom_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Compiler_Atom, Compiler_Object);

static void
Compiler_Atom_finalize
  (
    Shizu_State2* state,
    Compiler_Atom* self
  )
{
  if (self->bytes) {
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self->bytes);
    self->bytes = NULL;
  }
}

static void
Compiler_Atom_visit
  (
    Shizu_State2* state,
    Compiler_Atom* self
  )
{
  if (self->next) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->next);

  }
}

void
Compiler_Atom_construct
  (
    Shizu_State2* state,
    Compiler_Atom* self,
    Shizu_ByteArray* bytes
  )
{
  Shizu_Type* TYPE = Compiler_Atom_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
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

Compiler_Atom*
Compiler_Atom_create
  (
    Shizu_State2* state,
    Shizu_ByteArray* bytes
  )
{
  Shizu_Type* TYPE = Compiler_Atom_getType(state);
  Compiler_Atom* self = (Compiler_Atom*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Atom));
  Compiler_Atom_construct(state, self, bytes);
  return self;
}

static void
Compiler_Atoms_finalize
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  );

static void
Compiler_Atoms_visit
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  );

static Shizu_ObjectTypeDescriptor const Compiler_Atoms_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Atoms),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Atoms_visit,
  .finalize = (Shizu_OnFinalizeCallback*)&Compiler_Atoms_finalize,
  .dispatchSize = sizeof(Compiler_Atoms_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Compiler_Atoms, Compiler_Object);

static void
Compiler_Atoms_finalize
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  )
{
  if (self->buckets) {
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self->buckets);
    self->buckets = NULL;
  }
}

static void
Compiler_Atoms_visit
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  )
{
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Compiler_Atom* node  = self->buckets[i];
    while (node) {
      Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)node);
      node = node->next;
    }
  }
}

void
Compiler_Atoms_construct
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  )
{
  Shizu_Type* TYPE = Compiler_Atoms_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  self->buckets = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Compiler_Atom*) * 8);
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

Compiler_Atoms*
Compiler_Atoms_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Compiler_Atoms_getType(state);
  Compiler_Atoms* self = (Compiler_Atoms*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Atoms));
  Compiler_Atoms_construct(state, self);
  return self;
}
