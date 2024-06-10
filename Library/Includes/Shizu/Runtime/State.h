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



// Shizu includes.
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Configure.h"
#include "Shizu/Runtime/Type.h"

// Shizu forward declarations.
typedef struct Shizu_Environment Shizu_Environment;

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

void
Shizu_State_ensureModulesLoaded
  (
    Shizu_State* state
  );

Shizu_State1*
Shizu_State_getState1
  (
    Shizu_State* self
  );

Shizu_State2*
Shizu_State_getState2
  (
    Shizu_State* self
  );

Shizu_Environment*
Shizu_State_getGlobals
  (
    Shizu_State* state
  );

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* state,
    char const* bytes,
    size_t numberOfBytes
  );

/// @brief
/// Create a type.
/// @details
/// Create a type of the name @code{(bytes, numberOfBytes)}.
/// Raise an error if a type of that name already exists.
/// @param bytes
/// A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes
/// The number of Bytes in the array pointed to by @a bytes.
/// @param type
/// A pointer to the parent type or the null pointer.
/// @param dl
/// A pointer to the dynamic library the type descriptor is defined in. 
/// @param typeDestroyed
/// A pointer to the Shizu_OnTypeDestroyedCallback callback function.
/// @param descriptor
/// A pointer to the type descriptor.
Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* descriptor
  );

#endif // SHIZU_RUNTIME_STATE_H_INCLUDED
