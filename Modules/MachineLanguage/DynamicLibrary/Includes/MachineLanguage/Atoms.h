#if !defined(MACHINELANGUAGE_ATOMS_H_INCLUDED)
#define MACHINELANGUAGE_ATOMS_H_INCLUDED

#include "Shizu/Runtime/Include.h"

Shizu_declareObjectType(Atom);

struct Atom_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Atom {
  Shizu_Object _parent;
  Atom* next;
  uint8_t* bytes;
  size_t numberOfBytes;
  size_t hashValue;
};

void
Atom_construct
  (
    Shizu_State2* state,
    Atom* self,
    Shizu_ByteArray* bytes
  );

Atom*
Atom_create
  (
    Shizu_State2* state,
    Shizu_ByteArray* bytes
  );

Shizu_declareObjectType(Atoms);

struct Atoms_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Atoms {
  Shizu_Object _parent;
  Atom** buckets;
  size_t size;
  size_t capacity;
};

void
Atoms_construct
  (
    Shizu_State2* state,
    Atoms* self
  );

Atoms*
Atoms_create
  (
    Shizu_State2* state
  );

#endif // MACHINELANGUAGE_ATOMS_H_INCLUDED
