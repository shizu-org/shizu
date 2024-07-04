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

#if !defined(SHIZU_RUNTIME_STRING_H_INCLUDED)
#define SHIZU_RUNTIME_STRING_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/String.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"

Shizu_declareObjectType(Shizu_String);

/// @since 1.0
/// @brief Create a Shizu_String value from the specified sequence of Bytes.
/// @param state A pointer to a Shizu_State2 value.
/// @param bytes A pointer to an array of @a numberOfBytes bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @return A pointer to the Shizu_String value.
/// @undefined state does not point to a Shizu_State value.
/// @undefined bytes does not point to an array of @a numberOfBytes Bytes.
Shizu_String*
Shizu_String_create
  (
    Shizu_State2* state,
    char const* bytes,
    size_t numberOfBytes
  );

/// @since 0.1
/// @brief Concatenate two string.
/// @param state A pointer to a Shizu_State2 value.
/// @param self A pointer to this Shizu_String value.
/// @param other A pointer to another Shizu_String value.
/// @return The string representing the concatenation of the this Shizu_String value (the prefix) with the other Shizu_String value (the suffix).
Shizu_String*
Shizu_String_concatenate
  (
    Shizu_State2* state,
    Shizu_String* self,
    Shizu_String* other
  );

char const*
Shizu_String_getBytes
  (
    Shizu_State2* state,
    Shizu_String* self
  );

size_t
Shizu_String_getNumberOfBytes
  (
    Shizu_State2* state,
    Shizu_String* self
  );

Shizu_Boolean
Shizu_String_startsWith
  (
    Shizu_State2* state,
    Shizu_String* self,
    Shizu_String* prefix
  );

Shizu_Boolean
Shizu_String_endsWith
  (
    Shizu_State2* state,
    Shizu_String* self,
    Shizu_String* suffix
  );

#endif // SHIZU_RUNTIME_STRING_H_INCLUDED
