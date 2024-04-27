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

#if !defined(SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Gc.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Gc.h"

typedef struct Shizu_Gc {
  int referenceCount;
  Shizu_Object* all;
  Shizu_Object* gray;
} Shizu_Gc;

/// @since 1.0
/// Startup the "gc" module.
/// Called by Shizu_State_create/Shizu_State_destroy.
/// Shutdown the garbage collector module by calling Shizu_Gc_shutdown.
/// This function may invoke Shizu_State_(push|pop)JumpTarget, Shizu_State_(jump|setError|getError) hence that part of the Shizu_State object must be ready to use.
Shizu_Gc*
Shizu_Gc_startup
  (
    Shizu_State* state
  );

/// @since 1.0
/// Shutdown the "gc" module.
/// This function may only return via regular control flow and not via jump control flow.
void
Shizu_Gc_shutdown
  (
    Shizu_State* state,
    Shizu_Gc* gc
  );

/// @since 1.0
/// @brief Color a Shizu_Object value black.
/// @param object A pointer to the Shizu_Object value.
void
Shizu_Object_setWhite
  (
    Shizu_Object* object
  );

/// @since 1.0
/// @brief Get if an Shizu_Object value is colored white.
/// @param object A pointer to a Shizu_Object value.
/// @return @a true if the object is color gray. @a false otherwise.
bool
Shizu_Object_isWhite
  (
    Shizu_Object const* object
  );

/// @since 1.0
/// @brief Color a Shizu_Object value gray.
/// @param object A pointer to the Shizu_Object value.
void
Shizu_Object_setGray
  (
    Shizu_Object* object
  );

/// @since 1.0
/// @brief Get if an Shizu_Object value is colored gray.
/// @param object A pointer to a Shizu_Object value.
/// @return @a true if the object is color gray. @a false otherwise.
bool
Shizu_Object_isGray
  (
    Shizu_Object const* object
  );

/// @since 1.0
/// @brief Color a Shizu_Object value black.
/// @param object A pointer to the Shizu_Object value.
void
Shizu_Object_setBlack
  (
    Shizu_Object* object
  );

/// @since 1.0
/// @brief Get if an Shizu_Object value is colored black.
/// @param object A pointer to a Shizu_Object value.
/// @return @a true if the object is color black. @a false otherwise.
bool
Shizu_Object_isBlack
  (
    Shizu_Object const* object
  );

#endif // SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED
