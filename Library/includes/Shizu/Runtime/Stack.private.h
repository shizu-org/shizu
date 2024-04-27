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

#if !defined(SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Stack.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Stack.h"
#include "Shizu/Runtime/Gc.private.h"

/// @since 1.0
/// Startup the "stack" module.
/// Called by Shizu_State_create/Shizu_State_destroy.
/// Shutdown the stacks module by calling Shizu_Stack_shutdown.
/// This function requires the "gc" to be initialized.
Shizu_Stack*
Shizu_Stack_startup
  (
    Shizu_State* state
  );

/// @since 1.0
/// Shutdown the "stack" module.
/// This function may only return via regular control flow and not via jump control flow.
void
Shizu_Stack_shutdown
  (
    Shizu_State* state,
    Shizu_Stack* self
  );

/// @since 1.0
/// @brief Invoked to notify the "stack" module to perform pre-marking.
/// @param state A pointer to the Shizu_State object. 
void
Shizu_Stack_notifyPreMark
  (
    Shizu_State* state
  );

#endif // SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED
