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

#if !defined(SHIZU_RUNTIME_GC_H_INCLUDED)
#define SHIZU_RUNTIME_GC_H_INCLUDED

#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Value.h"

Shizu_declareType(Shizu_Object);

struct Shizu_Object_Dispatch {
  Shizu_Object_Dispatch* parent;
};

struct Shizu_Object {
  // Shizu_Object are kept in a singly-linked list.
  Shizu_Object* next;
  // Shizu_Object values are kept in the singly-linked "gray" list.
  Shizu_Object* gray;
  // Can assume the following values;
  // - 1 (decimal) or 01 (binary).
  // Indicates the color WHITE.
  // - 2 (decimal) or 10 (binary).
  // Indicates the color BLACK.
  // - 3 (decimal) or 01 | 10 (binary).
  // Indicates the color GRAY.
  uint8_t flags;
  // Pointer to the Shizu_Type object.
  Shizu_Type* type;
};

/// @brief Get the Shizu_Type value of a Shizu_Object value.
/// @param self A pointer to the Shizu_State object.
/// @param object A pointer the Shizu_Object value.
/// @return A pointer to the Shizu_Type value of the Shizu_Object value.
Shizu_Type*
Shizu_State_getObjectType
  (
    Shizu_State* self,
    Shizu_Object* object
  );

/// @brief Get the Shizu_Object_Dispatch value of a Shizu_Object value.
/// @param self A pointer to the Shizu_State object.
/// @param object A pointer the Shizu_Object value.
/// @return A pointer to the Shizu_Object_dispatch value of the Shizu_Object value.
Shizu_Object_Dispatch*
Shizu_State_getObjectDispatch
  (
    Shizu_State* state,
    Shizu_Object* object
  );

/// @since 1.0
/// Allocate an object of the specified size, in Bytes.
/// The object has the type "Shizu.Object" from which all other types are derived from.
Shizu_Object*
Shizu_Gc_allocate
  (
    Shizu_State* state,
    size_t size
  );

void
Shizu_Gc_visitObject
  (
    Shizu_State* state,
    Shizu_Object* object
  );

void
Shizu_Gc_visitValue
  (
    Shizu_State* state,
    Shizu_Value* value
  );

void
Shizu_Gc_run
  (
    Shizu_State* state
  );

#endif // SHIZU_RUNTIME_GC_H_INCLUDED
