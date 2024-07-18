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
#include "Shizu/Runtime/Object.h"
typedef struct Shizu_CxxProcedure Shizu_CxxProcedure;
typedef struct Shizu_Map Shizu_Map;
typedef struct Shizu_List Shizu_List;
typedef struct Shizu_String Shizu_String;
typedef struct Shizu_WeakReference Shizu_WeakReference;

/// @since 1.0
/// The CIL type
/// @code
/// class Shizu.Environment
/// @endcode
/// @details
/// The CIL type
/// @code
/// class Shizu.Environment
/// @endcode
/// The signature of the Shizu.Environment constructor is
/// @code
/// Shizu.Environment.construct()
/// @endcode
Shizu_declareObjectType(Shizu_Environment);

/// @since 1.0
/// @brief Get the size of this Shizu_Environment value.
/// @param state A pointer to a Shizu_State2 value.
/// @param self A poiner too this Shizu_Environment value.
/// @return The size of this Shizu_Environment object. Guaranteed to be smaller than or equal to the minimum of SIZE_MAX and Shizu_Integer32_Greatest.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Integer32
Shizu_Environment_getSize
  (
    Shizu_State2* state,
    Shizu_Environment* self
  );

/// @since 1.0
/// @brief Define (if not yet defined) ans assign a variable in this environment.
/// @param state A pointer to a Shizu_State2 value.
/// @param name The name of the variable.
/// @param value The value.
/// @success The variable is defined in this environment and has th specified value assigned.
/// @undefined @a state does not point to a Shizu_State value.
/// @error @a self is a null pointer.
/// @error @a name is a null pointer.
/// @error @a value is a null pointer.
void
Shizu_Environment_set
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Value* value
  );

/* Extension method. */
static inline void
Shizu_Environment_setBoolean
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Boolean value
  )
{
  Shizu_Value temporary;
  Shizu_Value_setBoolean(&temporary, value);
  Shizu_Environment_set(state, self, name, &temporary);
}

/* Extension method. */
static inline void
Shizu_Environment_setFloat32
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Float32 value
  )
{
  Shizu_Value temporary;
  Shizu_Value_setFloat32(&temporary, value);
  Shizu_Environment_set(state, self, name, &temporary);
}

/* Extension method. */
static inline void
Shizu_Environment_setInteger32
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Integer32 value
  )
{
  Shizu_Value temporary;
  Shizu_Value_setInteger32(&temporary, value);
  Shizu_Environment_set(state, self, name, &temporary);
}

/* Extension method. */
static inline void
Shizu_Environment_setObject
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Object* value
  )
{
  Shizu_Value temporary;
  Shizu_Value_setObject(&temporary, value);
  Shizu_Environment_set(state, self, name, &temporary);
}

/* Extension method. */
static inline void
Shizu_Environment_setVoid
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Void value
  )
{
  Shizu_Value temporary;
  Shizu_Value_setVoid(&temporary, value);
  Shizu_Environment_set(state, self, name, &temporary);
}

/// @error The variable is not defined in this environment.
/// @error @a self or @a key is null.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Value
Shizu_Environment_get
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_Boolean
Shizu_Environment_getBoolean
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/// @error The variable is not defined in this environment.
/// @error @a self, @a key, or @a type is null.
/// @error @a type does not denote a <code>Shizu.Object</code> or derived type.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Object*
Shizu_Environment_getObject
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name,
    Shizu_Type* type
  );

/* Extension method. */
Shizu_CxxProcedure*
Shizu_Environment_getCxxProcedure
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_Environment*
Shizu_Environment_getEnvironment
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_Integer32
Shizu_Environment_getInteger32
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_List*
Shizu_Environment_getList
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_Map*
Shizu_Environment_getMap
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_String*
Shizu_Environment_getString
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/* Extension method. */
Shizu_WeakReference*
Shizu_Environment_getWeakReference
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

/// @since 1.0
/// @brief Get if a variable is defined in this environment.
/// @param state A pointer to a Shizu_State2 value.
/// @return @a Shizu_Boolean_True if the variable is defined in this environment. @a Shizu_Boolean_False otherwise.
/// @undefined @a state does not point to a Shizu_State value.
Shizu_Boolean
Shizu_Environment_isDefined
  (
    Shizu_State2* state,
    Shizu_Environment* self,
    Shizu_String* name
  );

#endif // SHIZU_RUNTIME_OBJECTS_ENVIRONMENT_H_INCLUDED
