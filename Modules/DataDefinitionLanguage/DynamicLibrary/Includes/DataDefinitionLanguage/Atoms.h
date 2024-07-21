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

#if !defined(DATADEFINITIONLANGUAGE_ATOMS_H_INCLUDED)
#define DATADEFINITIONLANGUAGE_ATOMS_H_INCLUDED

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

#endif // DATADEFINITIONLANGUAGE_ATOMS_H_INCLUDED
