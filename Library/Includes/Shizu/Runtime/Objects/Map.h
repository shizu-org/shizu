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

#if !defined(SHIZU_RUNTIME_MAP_H_INCLUDED)
#define SHIZU_RUNTIME_MAP_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/Map.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"

/// @since 1.0
/// @brief
/// The Machine Language type
/// @code
/// class Shizu.Map
/// @endcode
/// @details
/// The constructor is
/// @code
/// Shizu.Map.construct()
/// @endcode
Shizu_declareObjectType(Shizu_Map);

/// @since 1.0
/// @brief Clear this map.
/// @param state A pointer to the Shizu_State2 object.
/// @param self A pointer to this Shizu_List object.
void
Shizu_Map_clear
  (
    Shizu_State2* state,
    Shizu_Map* self
  );

/// @since 1.0
/// @brief Get the size of this map.
/// @param state A pointer to the Shizu_State2 object.
/// @param self A pointer to this Shizu_Map object.
/// @return The size of this Shizu_Map object.
Shizu_Integer32
Shizu_Map_getSize
  (
    Shizu_State2* state,
    Shizu_Map* self
  );


/// @since 1.0
/// @brief Set/remove a pair in this map.
/// @param state A pointer to the Shizu_State2 object.
/// @param self A pointer to this Shizu_Map object.
/// @param key A pointer to the key value.
/// @param value A pointer to the value value.
/// @remarks
/// If <code>*key</code> stores a Void value, then this call immediatly returns.
/// Otherwise if <code>*value</code> stores a *Void* value, then this call is equivalent to Shizu_Map_remove(state, self, key).
/// Otherwise the pair <code>(*key,*value)</code> is added to this map.
void
Shizu_Map_set
  (
    Shizu_State2* state,
    Shizu_Map* self,
    Shizu_Value* key,
    Shizu_Value* value
  );

/// @since 1.0
/// @brief Get the value of a pair in this map.
/// @param state A pointer to the Shizu_State2 object.
/// @param self A pointer to this Shizu_Map object.
/// @remarks
/// If <code>key</code> stores the *Void* value or no entry for the pair was found then the *Void* value is returned.
/// otherwise the value of the pair is returned.
Shizu_Value
Shizu_Map_get
  (
    Shizu_State2* state,
    Shizu_Map* self,
    Shizu_Value* key
  );

#endif // SHIZU_RUNTIME_MAP_H_INCLUDED
