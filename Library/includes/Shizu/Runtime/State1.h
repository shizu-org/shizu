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

#if !defined(SHIZU_STATE1_H_INCLUDED)
#define SHIZU_STATE1_H_INCLUDED

#include "Shizu/Runtime/JumpTarget.h"
#include "Shizu/Runtime/Status.h"
#include "Shizu/Runtime/NoReturn.h"

// bool
#include <stdbool.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  #define Shizu_OperatingSystem_DlExtension ".dll"
  #define Shizu_OperatingSystem_DirectorySeparator "\\"

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem || Shizu_Configuration_OperatingSystem_Cygwin == Shizu_Configuration_OperatingSystem

  // NULL
  #include <stddef.h>

  #define Shizu_OperatingSystem_DlExtension ".so"
  #define Shizu_OperatingSystem_DirectorySeparator "/"

#else

  #error("operating system not (yet) supported")

#endif

/**
 * @since 1.0
 * The type of a dynamic library (DL).
 */
typedef struct Shizu_Dl Shizu_Dl;

/**
 * @since 1.0
 * The tier 1 state.
 * Provides
 * - get/set the error variable
 * - push pop on from the jump target stack, jumping to the top of the jump target stack
 * - get/set process exit flag
 */
typedef struct Shizu_State1 Shizu_State1;

/**
 * @since 1.0
 * @brief Acquire a refeference to the tier 1 state.
 * @param RETURN A pointer to a <code>Shizu_State1*</code> variable.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller acquire a reference to the <code>Shizu_State1</code> value.
 * The address of the value was assigned th <code>*RETURN</code>.
 */
int
Shizu_State1_acquire
  (
    Shizu_State1** RETURN
  );

/**
 * @since 1.0
 * @brief Relinquish a refeference to the tier 1 state.
 * @param self A pointer to a <code>Shizu_State1</code> value.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller relinquished his reference to the <code>Shizu_State1</code> value.
 */
int
Shizu_State1_relinquish
  (
    Shizu_State1* self
  );

/**
 * @since 1.0
 * @brief Push a jump target on the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param jumpTarget A pointer to the jump target.
 */
void
Shizu_State1_pushJumpTarget
  (
    Shizu_State1* state,
    Shizu_JumpTarget* jumpTarget
  );

/**
 * @since 1.0
 * @brief Pop a jump target from the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @pre The jump target stack is not empty.
 */
void
Shizu_State1_popJumpTarget
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Jump to the jump target at the top of the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @pre The jump target stack is not empty.
 */
Shizu_NoReturn() void
Shizu_State1_jump
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Set the status variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param status The value to assign to the variable.
 */
void
Shizu_State1_setStatus
  (
    Shizu_State1* state,
    Shizu_Status status
  );

/**
 * @since 1.0
 * @brief Get the status variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @return The value assigned to the variable.
 */
int
Shizu_State1_getStatus
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Set the process exit requested variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param processExitRequested The value to assign to the variable.
 */
void
Shizu_State1_setProcessExitRequested
  (
    Shizu_State1* state,
    bool processExitRequested
  );

/**
 * @since 1.0
 * @brief Get the process exist requested variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @return The value assigned to the variable.
 */
bool
Shizu_State1_getProcessExitRequested
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Get or load a DL.
 * @param state A pointer to the tier 1 state.
 * @param path The path to the DL (a C string).
 * @param load If @a true the DL is loaded if it is not loaded yet.
 * @return The DL on success if it exists. The null pointer oterwise.
 */
Shizu_Dl*
Shizu_State1_getOrLoadDl
  (
    Shizu_State1* state,
    char const* path,
    bool load
  );

/**
 * @since 1.0
 * @brief Get a DL by its name.
 * @param state A pointer to the tier 1 state.
 * @param name The name of the DL (as C string).
 * @return A pointer to the DL if a DL with the given name exists. The null pointer otherwise.
 */
Shizu_Dl*
Shizu_State1_getDlByName
  (
    Shizu_State1* state,
    char const* name
  );

/**
 * @since 1.0
 * @brief Get the DL which is defining a object.
 * @param state A pointer to the tier 1 state.
 * @param p The address of the object.
 * @return A pointer to the DL if a DL defining the object exists. the null pointer otherwise.
 */
Shizu_Dl*
Shizu_State1_getDlByAdr
  (
    Shizu_State1* state,
    void* p
  );

/**
 * @since 1.0
 * @brief Acquire a reference to a DL.
 * @param state A pointer to the tier 1 state.
 * @param dl A pointer to the DL.
 */
void
Shizu_State1_refDl
  (
    Shizu_State1* state,
    Shizu_Dl* dl
  );

/**
 * @since 1.0
 * @brief Relinquish a reference to a DL.
 * @param state A pinter to the tier 1 state.
 * @param dl A pointer to the DL.
 */
void
Shizu_State1_unrefDl
  (
    Shizu_State1* state,
    Shizu_Dl* dl
  );

/**
 * @since 1.0
 * @brief Load a symbol from a DL.
 * @param state A pointer to the tier 1 state.
 * @param dl A pointer to the DL.
 * @param name The name of the symbol (a C string).
 * @return A pointer to the symbol if it was found. The null pointer otherwise.
 */
void*
Shizu_State1_getDlSymbol
  (
    Shizu_State1* state,
    Shizu_Dl* dl,
    char const* name
  );

/// @brief Allocate named memory.
/// @param name The name.
/// @param n The size.
/// @error Named memory of the specified name already exists.
/// @error An allocation failed.
/// @error name is a null pointer.
/// @return A zero value on success. A non-zero value on failure.
int
Shizu_State1_allocateNamedStorage
  (
    Shizu_State1* state1,
    char const* name,
    size_t n
  );

/// @brief Deallocate named memory.
/// @param name The name of the named memory.
/// @return A zero value on success. A non-zero value on failure.
/// This function succeeds if the storage does not exist.
int
Shizu_State1_deallocateNamedStorage
  (
    Shizu_State1* state1,
    char const* name
  );

/// @brief Get type-scoped memory.
/// @param name The name of the named memory.
/// @param p A pointer to a <code>void*</code> value.
/// @return A zero value on success. A non-zero value on failure.
/// @success <code>*p</code> was assigned a pointer to the storage.
int
Shizu_State1_getNamedStorage
  (
    Shizu_State1* state1,
    char const* name,
    void** p
  );

#endif // SHIZU_STATE1_H_INCLUDED
