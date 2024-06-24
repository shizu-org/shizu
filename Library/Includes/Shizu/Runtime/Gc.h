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

#if !defined(SHIZU_RUNTIME_GC_H_INCLUDED)
#define SHIZU_RUNTIME_GC_H_INCLUDED

// size_t
#include <stddef.h>
#include "Shizu/Runtime/Value.h"
typedef struct Shizu_Gc Shizu_Gc;
typedef struct Shizu_State1 Shizu_State1;
typedef struct Shizu_State2 Shizu_State2;

/// @since 1.0
/// Allocate an object of the specified size, in Bytes.
/// The object has the type "Shizu.Object" from which all other types are derived from.
Shizu_Object*
Shizu_Gc_allocateObject
  (
    Shizu_State2* state,
    size_t size
  );

void
Shizu_Gc_visitObject
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Object* object
  );

void
Shizu_Gc_visitValue
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Value* value
  );

typedef struct Shizu_Gc_SweepInfo {
  size_t live;
  size_t dead;
} Shizu_Gc_SweepInfo;

typedef void Shizu_Gc_PreMarkCallbackContext;
typedef void (Shizu_Gc_PreMarkCallbackFunction)(Shizu_State1*, Shizu_Gc*, Shizu_Gc_PreMarkCallbackContext*);

/// @brief Add a registration of a pre mark hook.
/// @param context, notify The context and the function of the pre mark hook.
/// @remark The same @a context and @a notify pointers can be registered multiple times.
/// @undefined @a state does not point to a Shizu_State1 object.
/// @error @a gc is a null pointer.
/// @error @a function is a null pointer. 
void
Shizu_Gc_addPreMarkHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_PreMarkCallbackContext* context,
    Shizu_Gc_PreMarkCallbackFunction* function
  );

/// @brief Remove all(!) registrations of a pre mark hook.
/// @param context, notify The context and the function of the pre mark hook.
/// @undefined @a state does not point to a Shizu_State1 object.
/// @error @a gc is a null pointer.
/// @error @a function is a null pointer. 
void
Shizu_Gc_removePreMarkHook 
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_PreMarkCallbackContext* context,
    Shizu_Gc_PreMarkCallbackFunction* function
  );

typedef void Shizu_Gc_ObjectFinalizeCallbackContext;
typedef void (Shizu_Gc_ObjectFinalizeCallbackFunction)(Shizu_State1*, Shizu_Gc*, Shizu_Gc_ObjectFinalizeCallbackContext*, Shizu_Object*);

/// @brief Add a registration of a object finalize hook.
/// @param context, notify The context and the function of the object finalize hook.
/// @remark The same @a context and @a notify pointers can be registered multiple times.
/// @undefined @a state does not point to a Shizu_State1 object.
/// @error @a gc is a null pointer.
/// @error @a function is a null pointer. 
void
Shizu_Gc_addObjectFinalizeHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_ObjectFinalizeCallbackContext* context,
    Shizu_Gc_ObjectFinalizeCallbackFunction* function
  );

/// @brief Remove all(!) registrations of an object finalize hook.
/// @param context, notify The context and the function of the object finalize hook.
/// @undefined @a state does not point to a Shizu_State1 object.
/// @error @a gc is a null pointer.
/// @error @a function is a null pointer. 
void
Shizu_Gc_removeObjectFinalizeHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_ObjectFinalizeCallbackContext* context,
    Shizu_Gc_ObjectFinalizeCallbackFunction* function
  );

void
Shizu_Gc_run
  (
    Shizu_State2* state,
    Shizu_Gc* self,
    Shizu_Gc_SweepInfo* sweepInfo
  );

#endif // SHIZU_RUNTIME_GC_H_INCLUDED
