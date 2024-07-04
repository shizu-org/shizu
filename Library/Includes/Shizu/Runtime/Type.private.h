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
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Type.h"
#include "Shizu/Runtime/Types/_SmallTypeArray.h"

/// This flag is set for the "Object" and derived type types.
#define Shizu_TypeFlags_ObjectType (1)

/// This flag is set for "Boolean", "CxxFunction", "Float32", "Integer32", and "Void" types.
#define Shizu_TypeFlags_PrimitiveType (2)

/// This flag is set after a succesfull call to Shizu_OnPostCreateTypeCallback function for a type.
/// If the call is not successful, this flag is not set in that type.
/// If this flag is set in a type before that type is destroyed, the Shizu_OnPreDestroyTypeCallback function (if any) is invoked and this flag is cleared for that type.
#define Shizu_TypeFlags_PostTypeCreationInvoked (4)

/// This flag is set after a successful call to the Shizu_OnDispatchInitialize function of the type. If the call is not successful, the flag is not set.
/// A successful call to Shizu_OnDispatchUninitialize clears the flag.
#define Shizu_TypeFlags_DispatchInitialized (8)

typedef struct Shizu_Type Shizu_Type;

typedef struct Shizu_Types Shizu_Types;

typedef struct Shizu_PrimitiveTypeNode {
  // Pointer to the type descriptor.
  Shizu_PrimitiveTypeDescriptor const* descriptor;
} Shizu_PrimitiveTypeNode;

typedef struct Shizu_ObjectTypeNode {
  // Pointer to the type descriptor.
  Shizu_ObjectTypeDescriptor const* descriptor;
  // A pointer to the dispatch if Shizu_TypeFlags_DispatchInitialized is set in flags.
  // The null pointer otherwise.
  Shizu_Object_Dispatch* dispatch;
  // The parent type of this type (null only for the "Object" type).
  Shizu_Type* parentType;
  // The array of pointers to child types of this type.
  SmallTypeArray children;
} Shizu_ObjectTypeNode;

struct Shizu_Type {
  Shizu_Type* next;
  // The flags of this type.
  uint8_t flags;
  // The name of this type.
  struct {
    size_t hashValue;
    // The number of Bytes including the zero terminator.
    size_t numberOfBytes;
    // A pointer to an array of @a numberOfBytes containing a zero-terminated C string.
    char* bytes;
  } name;
  // A pointer to the DL from which the type originates from or the null pointer.
  Shizu_Dl* dl;
  // A pointer to a "type destroyed" callback function or the null pointer.
  Shizu_OnTypeDestroyedCallback* typeDestroyed;

  union {
    Shizu_ObjectTypeNode objectType;
    Shizu_PrimitiveTypeNode primitiveType;
  };
};

struct Shizu_Types{
  Shizu_Type** elements;
  size_t size;
  size_t capacity;
};

Shizu_Types*
Shizu_Types_startup
  (
    Shizu_State1* state1
  );

void
Shizu_Types_shutdown
  (
    Shizu_State1* state1,
    Shizu_Types* self
  );

void
Shizu_Types_initialize
  (
    Shizu_State1* state1,
    Shizu_Types* self
  );

void
Shizu_Types_uninitialize
  (
    Shizu_State1* state1,
    Shizu_Types* self
  );

// Ensure the type scoped data is initialized.
// If the Shizu_TypeFlags_PostCreateTypeInvoked flag is not set, then this function will invoke the Shizu_OnPostcreateTypeCallback if it exists.
// If this function does not exist or the call to this function succeeds, it will set the Shizu_TypeFlags_PostCreateTypeInvoked flag.
void
Shizu_Types_onPostCreateType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

// Ensure the type scoped data is uninitialized.
// If the Shizu_TypeFlags_PostCreateTypeInvoked is set, this function invokes the Shizu_OnPreDestroyTypeCallback if it exists.
// If this function does not exist or the call to this function succeeds, it will set clear the Shizu_TypeFlags_PostCreateTypeInvoked flag.
void
Shizu_Types_onPreDestroyType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

// This ensure that the dispatch of a type is initialized.
void
Shizu_Types_ensureDispatchInitialized
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* type
  );

// This ensures that the dispatch of a type and all its descendant types are uninitialized.
// It is guaranteed that the dispatch of a descendant type is uninitialized before the dispatch of an ancestor type is uninitialized.
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
    Shizu_State1* state1,
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

#endif // SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED
