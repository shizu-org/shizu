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

#if !defined(SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/WeakReference.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Objects/WeakReference.h"
#include "Shizu/Runtime/Gc.private.h"
  
/// @since 1.0
/// @internal
/// @brief Notify the weak references on an "Shizu.Object Destroyed" event.
/// @param state A pointer to the Shizu_State value.
/// @param reference A pointer to the Shizu_Object value that was destroyed.
/// @remarks This will search the bucket where the WeakReference nodes for this reference reside.
/// Next it sets any reference in all nodes refering to that object to null.
void
Shizu_WeakReferences_notifyDestroy
	(
		Shizu_State* state,
		Shizu_Object* reference
	);

/// @since 1.0
/// @brief Get the referenced Shizu.Object if it is still alive.
/// @param state A pointer to the Shizu_State value.
/// @param self A ppointer to the Shizu_WeakReference value.
/// @return A pointer to the Shizu.Object if it is still alive. A null pointer otherwise.
Shizu_Object*
Shizu_WeakReference_get
  (
    Shizu_State* state,
    Shizu_WeakReference* self
  );

#endif // SHIZU_RUNTIME_WEAKREFERENCE_PRIVATE_H_INCLUDED
