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

#if !defined(SHIZU_RUNTIME_WEAKREFERENCE_H_INCLUDED)
#define SHIZU_RUNTIME_WEAKREFERENCE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/WeakReference.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"

/// The "weak references" state.
/// @todo Rename to "Shizu_WeakReferencesState".
typedef struct Shizu_WeakReferences Shizu_WeakReferences;

Shizu_declareObjectType(Shizu_WeakReference);

/// @since 1.0
/// @brief Create a Shizu_WeakReference value.
/// @param state A pointer to a Shizu_State2 object.
/// @param reference A pointer to a Shizu_Object value or a null pointer.
/// @return A pointer to the Shizu_WeakReference value.
Shizu_WeakReference*
Shizu_WeakReference_create
  (
    Shizu_State2* state,
    Shizu_Object* reference
  );

#endif // SHIZU_RUNTIME_WEAKREFERENCE_H_INCLUDED
