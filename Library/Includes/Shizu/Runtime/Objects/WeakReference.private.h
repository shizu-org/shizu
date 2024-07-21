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

#if !defined(SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/WeakReference.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Objects/WeakReference.h"
typedef struct Shizu_Gc Shizu_Gc;

/// @since 1.0
/// Startup the "weak references" state.
/// Called by Shizu_State2_create/Shizu_State2_destroy.
/// Shutdown the "weak references" state by calling Shizu_WeakReferences_destroy.
/// This function may invoke Shizu_State1_(push|pop)JumpTarget, Shizu_State1_(jump|setStatus|getStatus) Shizu_State1 is required.
/// Only one Shizu_WeakReferences object may exist in a process.
/// @remarks This function requires the "gc" state to be available.
Shizu_WeakReferences*
Shizu_WeakReferences_create
  (
    Shizu_State1* state
  );

/// @since 1.0
/// Shutdown the "weak references" module.
/// This function may only return via regular control flow and not via jump control flow.
void
Shizu_WeakReferences_destroy
  (
    Shizu_State1* state,
    Shizu_WeakReferences* self
  );

void
Shizu_WeakReferenceState_notifyObjectFinalize
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_WeakReferences* self,
    Shizu_Object* reference
  );

#endif // SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED
