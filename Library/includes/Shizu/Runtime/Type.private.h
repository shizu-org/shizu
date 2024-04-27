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

#if !defined(SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Type.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.private.h"

// This flag is set after a successful call to the Shizu_OnStaticInitialize function of the type. If the call is not successful, the flag is not set.
// A successful call to Shizu_OnStaticUninitialize clears the flag.
#define Shizu_TypeFlags_StaticallyInitialized (1)

// This flag is set after a successful call to the Shizu_OnDispatchInitialize function of the type. If the call is not successful, the flag is not set.
// A successful call to Shizu_OnDispatchUninitialize clears the flag.
#define Shizu_TypeFlags_DispatchInitialized (2)

typedef struct Shizu_Type Shizu_Type;

typedef struct Shizu_Types Shizu_Types;

struct Shizu_Type {
  Shizu_Type* next;
  Shizu_Type* parentType;
  uint8_t flags;
  struct {
    size_t hashValue;
    // The number of Bytes including the zero terminator.
    size_t numberOfBytes;
    // A pointer to an array of @a numberOfBytes containing a zero-terminated C string.
    char* bytes;
  } name;
  Shizu_OnTypeDestroyedCallback* typeDestroyed;
  Shizu_TypeDescriptor const* descriptor;
  // Capacity is the size of the array.
  // The first n <= capacity entries point to elements, the remaining elements are NULL. 
  struct {
    Shizu_Type** elements;
    size_t capacity;
  } children;
  // A pointer to the DL from which the type originates from or the null pointer.
  Shizu_Dl* dl;
  // A pointer to the dispatch if Shizu_TypeFlags_DispatchInitialized is set in flags.
  // The null pointer otherwise.
  Shizu_Object_Dispatch* dispatch;
};

struct Shizu_Types{
  Shizu_Type** elements;
  size_t size;
  size_t capacity;
};

void
Shizu_Types_initialize
  (
    Shizu_State1* state1,
    Shizu_Types* self
  );

void
Shizu_Types_uninitialize
  (
    Shizu_State* self
  );

void
Shizu_Types_ensureDispatchInitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

void
Shizu_Types_ensureDispatchUninitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

// The type must already be removed from the Shizu_Types hash table.
// Furthermore, the type must have zero children.
void
Shizu_Type_destroy
  (  
    Shizu_State* state,
    Shizu_Types* self,
    Shizu_Type* type
  );

/**
 * @brief Get the number of child types of a type.
 * @param state1 A pointer to a Shizu_State1 object.
 * @param self A pointer to the Shizu_Types object.
 * @param type A pointer to the type.
 * @return The number of child types of the type.
 */
size_t
Shizu_Types_getTypeChildCount
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

/**
 * @since 1.0
 * Get a type by its name.
 * @param state1 A pointer to a Shizu_State1 object.
 * @param self A pointer to the Shizu_Types object.
 * @param name A pointer to the name, a C string.
 * @return A pointer to the type if it was found. The null pointer otherwise.
 */
Shizu_Type*
Shizu_Types_getTypeByName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* name
  );

Shizu_Type*
Shizu_Types_createType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* typeDescriptor
  );

#endif // SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED
