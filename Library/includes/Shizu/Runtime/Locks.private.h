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

#if !defined(SHIZU_RUNTIME_LOCKS_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_LOCKS_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Locks.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Locks.h"

/// @since 1.0
/// Startup the "locks" module.
/// Called by Shizu_State_create/Shizu_State_destroy.
/// Shutdown the locks module by calling Shizu_Locks_shutdown.
/// This function requires the "gc" module to be initialized.
Shizu_Locks*
Shizu_Locks_startup
  ( 
    Shizu_State* state
  );

/// @since 1.0
/// Shutdown the "locks" module.
/// This function may only return via regular control flow and not via jump control flow.
void
Shizu_Locks_shutdown
  (
    Shizu_State* state,
    Shizu_Locks* self
  );

/// @since 1.0
/// @brief Invoked to notify the "locks" module to perform pre-marking.
/// @param state A pointer to the Shizu_State object. 
void
Shizu_Locks_notifyPreMark
  (
    Shizu_State* state
  );

void
Shizu_Locks_notifyDestroy
  (
    Shizu_State* state,
    Shizu_Object* object
  );

#endif // SHIZU_RUNTIME_LOCKS_PRIVATE_H_INCLUDED
