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

#if !defined(SHIZU_RUNTIME_STATE_H_INCLUDED)
#define SHIZU_RUNTIME_STATE_H_INCLUDED



#define _GNU_SOURCE

// setjmp, jmp_buf, longjmp
#include <setjmp.h>

// bool
#include <stdbool.h>



// Shizu includes.
#include "Shizu/Runtime/Configure.h"
#include "Shizu/Runtime/JumpTarget.h"
#include "Shizu/Runtime/NoReturn.h"
#include "Shizu/Runtime/Status.h"
#include "Shizu/Runtime/Type.h"

// Shizu forward declaration.
typedef struct Shizu_Dl Shizu_Dl;



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



typedef struct Shizu_Object Shizu_Object;

typedef struct Shizu_State Shizu_State;

typedef struct Shizu_Gc Shizu_Gc;

typedef struct Shizu_Locks Shizu_Locks;

typedef struct Shizu_Stack Shizu_Stack;



int
Shizu_State_create
  (
    Shizu_State** RETURN
  );

int
Shizu_State_destroy
  (
    Shizu_State* self
  );

void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  );

void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  );

void
Shizu_State_setStatus
  (
    Shizu_State* self,
    Shizu_Status status
  );

Shizu_Status
Shizu_State_getStatus
  (
    Shizu_State* self
  );

Shizu_NoReturn() void
Shizu_State_jump
  (
    Shizu_State* self
  );

void
Shizu_State_setProcessExitRequested
  (
    Shizu_State* state,
    bool processExitRequested
  );

bool
Shizu_State_getProcessExitRequested
  (
    Shizu_State* state
  );



Shizu_Gc*
Shizu_State_getGc
  (
    Shizu_State* self
  );

Shizu_Locks*
Shizu_State_getLocks
  (
    Shizu_State* self
  );

Shizu_Stack*
Shizu_State_getStack
  (
    Shizu_State* self
  );

Shizu_Types*
Shizu_State_getTypes
  (
    Shizu_State* self
  );

/// @brief Allocate named memory.
/// @param name The name.
/// @param n The size.
/// @error Named memory of the specified name already exists.
/// @error An allocation failed.
/// @error name is a null pointer.
/// @return A zero value on success. A non-zero value on failure.
int
Shizu_State_allocateNamedMemory
  (
    Shizu_State* state,
    char const* name,
    size_t n
  );

/// @brief Deallocate named memory.
/// @param name The name of the named memory.
int
Shizu_State_deallocateNamedMemory
  (
    Shizu_State* state,
    char const* name
  );

/// @brief Get type-scoped memory.
/// @param name
/// @error no memory of the specified ID exists.
int
Shizu_State_getNamedMemory
  (
    Shizu_State* state,
    char const* name,
    void** p
  );

Shizu_Dl*
Shizu_State_getOrLoadDl
  (
    Shizu_State* state,
    char const* pathName,
    bool loadDll
  );

Shizu_Dl*
Shizu_State_getDlByName
  (
    Shizu_State* state,
    char const* name
  );

Shizu_Dl*
Shizu_State_getDlByAdr
  (
    Shizu_State* state,
    void* p
  );

void
Shizu_Dl_ref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  );

void
Shizu_Dl_unref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  );

void*
Shizu_Dl_getSymbol
  (
    Shizu_State* state,
    Shizu_Dl* dl,
    char const* name
  );

#endif // SHIZU_RUNTIME_STATE_H_INCLUDED
