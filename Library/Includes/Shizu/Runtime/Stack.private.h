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

#if !defined(SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Stack.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Stack.h"
typedef struct Shizu_Gc Shizu_Gc;

/// @since 1.0
/// @state-constructor
/// Startup the "stack" module.
/// Called by Shizu_State_create/Shizu_State_destroy.
/// Shutdown the "stack" module by calling Shizu_Stack_shutdown.
/// This function may invoke Shizu_State1_(push|pop)JumpTarget, Shizu_State1_(jump|setStatus|getStatus) Shizu_State1 is required.
/// Only one Shizu_Stack object may exist in a process.
/// @remarks This function requires the "gc" state to be available.
Shizu_Stack*
Shizu_Stack_create
  (
    Shizu_State1* state
  );

/// @since 1.0
/// @state-destructor
/// @brief Shutdown the "stack" state.
/// @param state A pointer to a Shizu_State2 value. 
/// This function returns only via regular control flow and not via jump control flow.
void
Shizu_Stack_destroy
  (
    Shizu_State1* state,
    Shizu_Stack* self
  );

/// @since 1.0
/// @brief Invoked to notify the "stack" module to perform pre-marking.
/// @param state A pointer to a Shizu_State2 value. 
void
Shizu_Stack_notifyPreMark
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Stack* self
  );

#endif // SHIZU_RUNTIME_STACK_PRIVATE_H_INCLUDED
