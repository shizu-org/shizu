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

#if !defined(SHIZU_RUNTIME_MODULE_H_INCLUDED)
#define SHIZU_RUNTIME_MODULE_H_INCLUDED

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Gc.h"
#include "Shizu/Runtime/Object.h"
typedef struct Shizu_String Shizu_String;

Shizu_declareObjectType(Shizu_Module);

struct Shizu_Module_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Shizu_Module {
  Shizu_Object _parent;

  // The path to the module dl file.
  Shizu_String* path;

  // Pointer to the dl object if the dl is loaded. The null pointer otherwise.
  Shizu_Dl* dl;
};

Shizu_Module*
Shizu_Module_create
  (
    Shizu_State2* state,
    Shizu_String* folderPath
  );

Shizu_String*
Shizu_Module_getName
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  );

Shizu_CxxFunction*
Shizu_Module_getUpdate
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  );

/// @brief Ensure the module is loaded.
/// @detail
/// Load the module if it was not loaded already.
/// Do nothing if the module was already loaded.
/// @param state A pointer to a Shizu_State2 state.
/// @param self A pointer to this module.
void
Shizu_Module_ensureLoaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  );

/// @brief Ensure the module is not loaded.
/// @detail
/// Unload the module if it was loaded.
/// Do nothing if the module was not loaded.
/// @param state A pointer to a Shizu_State2 state.
/// @param self A pointer to this module.
void
Shizu_Module_ensureUnloaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  );

/// @brief Get if this module is loaded.
/// @param state A pointer to a Shizu_State2 state.
/// @param self A pointer to this module.
/// @return @a Shizu_Boolean_True if this module is loaded. @a Shizu_boolean_False if this module is not loaded.
Shizu_Boolean
Shizu_Module_isLoaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  );

#endif // SHIZU_RUNTIME_MODULE_H_INCLUDED
