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

#if !defined(SHIZU_RUNTIME_CXXPROCEDURE_H_INCLUDED)
#define SHIZU_RUNTIME_CXXPROCEDURE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/CxxProcedure.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"

/// @todo
/// Add calling modes:
/// "tier2" (requires at least tier 2 state)
/// "tier3" (requires at least tier 3 state)
Shizu_declareObjectType(Shizu_CxxProcedure);

struct Shizu_CxxProcedure_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Shizu_CxxProcedure {
  Shizu_Object _parent;
  Shizu_CxxFunction* f;
  Shizu_Dl* dl;
};

/// @since 1.0
/// @brief
/// Construct a Shizu_CxxProcedure value.
/// @param state
/// A pointer to a Shizu_State2 value.
/// @param self
/// A pointer to the Shizu_CxxProcedure object.
/// @param f
/// A pointer to a Shizu_CxxFunction value.
/// @param dl
/// The null pointer if the Shizu_CxxFunction value is defined in the main module.
/// A pointer to the Shizu_Dl object in which the Cxx function is defined.
/// @remarks
/// The behavior of this constructor is undefined if any of the following conditions is true:
/// - @a f does not point to Shizu_CxxFunction value.
/// - @a dl is not a null value and does not point to the DL object in which the Shhizu_CxxFunction object pointed to by @a f is defined.
/// - @a state does not point to a Shizu_State object.
void
Shizu_CxxProcedure_construct
  (
    Shizu_State2* state,
    Shizu_CxxProcedure* self,
    Shizu_CxxFunction* f,
    Shizu_Dl* dl
  );

/// @since 1.0
/// @brief
/// Create a Shizu_CxxProcedure value.
/// @param state
/// A pointer to a Shizu_State2 value.
/// @param f
/// A pointer to a Shizu_CxxFunction value.
/// @param dl
/// The null pointer if the Shizu_CxxFunction value is defined in the main module.
/// A pointer to the Shizu_Dl object in which the Cxx function is defined.
/// @return
/// A pointer to the Shizu_CxxProcedure value.
/// @remarks
/// The behavior of this constructor is undefined if any of the following conditions is true:
/// - @a f does not point to Shizu_CxxFunction value.
/// - @a dl is not a null value and does not point to the DL object in which the Shhizu_CxxFunction object pointed to by @a f is defined.
/// - @a state does not point to a Shizu_State object.
/// @remarks
/// Shizu.CxxProcedure has a native "create" function as it is means to transfer data from the C/C++ domain to the X domain.
Shizu_CxxProcedure*
Shizu_CxxProcedure_create
  (
    Shizu_State2* state,
    Shizu_CxxFunction* f,
    Shizu_Dl* dl
  );

#endif // SHIZU_RUNTIME_CXXPROCEDURE_H_INCLUDED
