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

#if !defined(SHIZU_RUNTIME_STATE2_H_INCLUDED)
#define SHIZU_RUNTIME_STATE2_H_INCLUDED

// Shizu includes.
#include "Shizu/Runtime/State1.h"

// Shizu forward declarations.
typedef struct Shizu_Environment Shizu_Environment;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_WeakReference Shizu_WeakReference;

typedef struct Shizu_WeakReferences Shizu_WeakReferences;
typedef struct Shizu_Gc Shizu_Gc;
typedef struct Shizu_Locks Shizu_Locks;
typedef struct Shizu_Stack Shizu_Stack;
typedef struct Shizu_Types Shizu_Types;

/**
 * @since 1.0
 * @brief A type of a "tier 2" state.
 */
typedef struct Shizu_State2 Shizu_State2;

/**
 * @since 1.0
 * @brief Acquire a refeference to the tier 2 state.
 * @param RETURN A pointer to a <code>Shizu_State2*</code> variable.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller acquire a reference to the <code>Shizu_State2</code> value.
 * The address of the value was assigned th <code>*RETURN</code>.
 */
int
Shizu_State2_acquire
  (
    Shizu_State2** RETURN
  );

/**
 * @since 1.0
 * @brief Relinquish a refeference to the tier 2 state.
 * @param self A pointer to a <code>Shizu_State2</code> value.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller relinquished his reference to the <code>Shizu_State2</code> value.
 */
int
Shizu_State2_relinquish
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Push a jump target on the jump target stack.
 * @param self A pointer to this state.
 * @param jumpTarget A pointer to a stack allocated jump target.
 * @undefined @a self does not point to a state.
 * @undefined @a jumpTarget does not point to a stack allocated jump target.
 */
void
Shizu_State2_pushJumpTarget
  (
    Shizu_State2* self,
    Shizu_JumpTarget* jumpTarget
  );

/**
 * @since 1.0
 * @brief Pop a jump target from the jump target stack.
 * @param self A pointer to this state.
 * @undefined @a self does not point to a state.
 * @udefined The jump target stack is empty.
 */
void
Shizu_State2_popJumpTarget
(
  Shizu_State2* self
);

/**
 * @since 1.0
 * @brief Set the status variable.
 * @param self A pointer to this state.
 * @param status The value to set.
 * @undefined @a self does not point to a state. 
 */
void
Shizu_State2_setStatus
  (
    Shizu_State2* self,
    Shizu_Status status
  );

Shizu_Status
Shizu_State2_getStatus
  (
    Shizu_State2* self
  );

Shizu_NoReturn() void
Shizu_State2_jump
  (
    Shizu_State2* self
  );

void
Shizu_State2_setProcessExitRequested
  (
    Shizu_State2* self,
    bool processExitRequested
  );

bool
Shizu_State2_getProcessExitRequested
  (
    Shizu_State2* self
  );

Shizu_State1*
Shizu_State2_getState1
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Get the "types" state component.
 * @param self A pointer to this state.
 * @return A pointer to the "types" state component.
 * The null pointer if the "types" component does not exist.
 */
Shizu_Types*
Shizu_State2_getTypes
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Get the "gc" state component.
 * @param self A pointer to this state.
 * @return A pointer to the "gc" state component.
 * The null pointer if the "gc" component does not exist.
 */
Shizu_Gc*
Shizu_State2_getGc
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Get the "locks" state component.
 * @param self A pointer to this state.
 * @return A pointer to the "locks" state component.
 * The null pointer if the "locks" component does not exist.
 */
Shizu_Locks*
Shizu_State2_getLocks
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Get the "stack" state component.
 * @param self A pointer to this state.
 * @return A pointer to the "stack" state component.
 * The null pointer if the "stack" component does not exist.
 */
Shizu_Stack*
Shizu_State2_getStack
  (
    Shizu_State2* self
  );

/**
 * @since 1.0
 * @brief Get the "weak references" state component.
 * @param self A pointer to this state.
 * @return A pointer to the "weak references" state component.
 * The null pointer if the "weak references" component does not exist.
 */
Shizu_WeakReferences*
Shizu_State2_getWeakReferences
  (
    Shizu_State2* self
  );

Shizu_Environment*
Shizu_State2_getGlobalEnvironment
  (
    Shizu_State2* self
  );

void
Shizu_State2_ensureModulesLoaded
  (
    Shizu_State2* state
  );

#endif // SHIZU_RUNTIME_STATE2_H_INCLUDED
