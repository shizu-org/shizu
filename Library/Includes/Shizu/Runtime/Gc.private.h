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

#if !defined(SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Gc.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Gc.h"
#include "Shizu/Gc/Include.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct PreMarkHookNode PreMarkHookNode;

struct PreMarkHookNode {
  PreMarkHookNode* next;
  bool dead;
  Shizu_Gc_PreMarkCallbackContext* context;
  Shizu_Gc_PreMarkCallbackFunction* function;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct ObjectFinalizeHookNode ObjectFinalizeHookNode;

struct ObjectFinalizeHookNode {
  ObjectFinalizeHookNode* next;
  bool dead;
  Shizu_Gc_ObjectFinalizeCallbackContext* context;
  Shizu_Gc_ObjectFinalizeCallbackFunction* function;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Shizu_Gc Shizu_Gc;

struct Shizu_Gc {
  int referenceCount;
  /// The GCX type for "Shizu.Object".
  Shizu_Gcx_Type* type;
  Shizu_Object* all;
  Shizu_Object* gray;
  struct {
    PreMarkHookNode* nodes;
    bool running;
  } preMarkHooks;
  struct {
    ObjectFinalizeHookNode* nodes;
    bool running;
  } objectFinalizeHooks;
};

/// @since 1.0
/// @state-constructor
/// Startup the "garbage collector" state.
/// Called by Shizu_State2_create/Shizu_State2_destroy.
/// Shutdown the "garbage collector" state by calling Shizu_Gc_destroy.
/// This function may invoke Shizu_State1_(push|pop)JumpTarget, Shizu_State1_(jump|setStatus|getStatus) Shizu_State1 is required.
/// Only one Shizu_Gc object may exist in a process.
Shizu_Gc*
Shizu_Gc_create
  (
    Shizu_State2* state
  );

/// @since 1.0
/// @state-destructor
/// Shutdown the "garbage collector" state.
/// This function may only return via regular control flow and not via jump control flow.
void
Shizu_Gc_destroy
  (
    Shizu_State2* state,
    Shizu_Gc* self
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
/// @return @a true if the object is colored gray. @a false otherwise.
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
/// @return @a true if the object is colored gray. @a false otherwise.
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
/// @return @a true if the object is colored black. @a false otherwise.
bool
Shizu_Object_isBlack
  (
    Shizu_Object const* object
  );

#endif // SHIZU_RUNTIME_GC_PRIVATE_H_INCLUDED
