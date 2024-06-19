#if !defined(COMPILER_ATOMS_H_INCLUDED)
#define COMPILER_ATOMS_H_INCLUDED

#include "Compiler/Object.h"

Shizu_declareType(Compiler_Atom);

struct Compiler_Atom_Dispatch {
  Compiler_Object_Dispatch _parent;
};

struct Compiler_Atom {
  Compiler_Object _parent;
  Compiler_Atom* next;
  uint8_t* bytes;
  size_t numberOfBytes;
  size_t hashValue;
};

void
Compiler_Atom_construct
  (
    Shizu_State2* state,
    Compiler_Atom* self,
    Shizu_ByteArray* bytes
  );

Compiler_Atom*
Compiler_Atom_create
  (
    Shizu_State2* state,
    Shizu_ByteArray* bytes
  );

Shizu_declareType(Compiler_Atoms);

struct Compiler_Atoms_Dispatch {
  Compiler_Object_Dispatch _parent;
};

struct Compiler_Atoms {
  Compiler_Object _parent;
  Compiler_Atom** buckets;
  size_t size;
  size_t capacity;
};

void
Compiler_Atoms_construct
  (
    Shizu_State2* state,
    Compiler_Atoms* self
  );

Compiler_Atoms*
Compiler_Atoms_create
  (
    Shizu_State2* state
  );

#endif // COMPILER_ATOMS_H_INCLUDED
