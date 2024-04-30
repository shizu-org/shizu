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

#if !defined(SHIZU_RUNTIME_OBJECTS_ENVIRONMENT_H_INCLUDED)
#define SHIZU_RUNTIME_OBJECTS_ENVIRONMENT_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
#error("Do not include `Shizu/Runtime/Objects/Environment.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Value.h"

Shizu_declareType(Shizu_Environment);

/// @since 1.0
/// @brief Create a Shizu_Environment value.
/// @param state A pointer to a Shizu_State value.
/// @return A pointer to the Shizu_Environment value.
/// @undefined state does not point to a Shizu_State value.
Shizu_Environment*
Shizu_Environment_create
  (
    Shizu_State* state
  );

/// @since 1.0
/// @brief Get the size of this Shizu_Environment value.
/// @param state A pointer to a Shizu_State value.
/// @param self A poiner too this Shizu_Environment value.
/// @return The size of this Shizu_Environment object. Guaranteed to be smaller than or equal to the minimum of SIZE_MAX and Shizu_Integer32_Greatest.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Integer32
Shizu_Environment_getSize
  (
    Shizu_State* state,
    Shizu_Environment* self
  );

/// @since 1.0
/// @brief Define a variable in this environment.
/// @param state A pointer to a Shizu_State value.
/// @success The variable is defined in this environment.
/// @error The variable is already defined in this environment.
/// @undefined @a state does not point to a Shizu_State value.
void
Shizu_Environment_define
  (
    Shizu_State* state,
    Shizu_Environment* self,
    Shizu_String* key
  );

/// @since 1.0
/// @brief Get if a variable is defined in this environment.
/// @param state A pointer to a Shizu_State value.
/// @return @a Shizu_Boolean_True if the variable is defined in this environment. @a Shizu_Boolean_False otherwise.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Boolean
Shizu_Environment_ísDefined
  (
    Shizu_State* state,
    Shizu_Environment* self,
    Shizu_String* key
  );

#endif // SHIZU_RUNTIME_OBJECTS_ENVIRONMENT_H_INCLUDED
