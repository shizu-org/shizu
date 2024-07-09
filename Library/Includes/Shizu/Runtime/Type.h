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

#if !defined(SHIZU_RUNTIME_TYPE_H_INCLUDED)
#define SHIZU_RUNTIME_TYPE_H_INCLUDED

#include "Shizu/Runtime/Configure.h"
#include "Shizu/Runtime/Value.h"

// bool
#include <stdbool.h>
// size_t
#include <stddef.h>

typedef struct Shizu_State1 Shizu_State1;
typedef struct Shizu_State2 Shizu_State2;
typedef struct Shizu_Dl Shizu_Dl;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Object_Dispatch Shizu_Object_Dispatch;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_ObjectTypeDescriptor Shizu_ObjectTypeDescriptor;
typedef struct Shizu_PrimitiveTypeDescriptor Shizu_PrimitiveTypeDescriptor;
typedef struct Shizu_Types Shizu_Types;

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_DlExport() __declspec(dllexport)
#else
  #define Shizu_DlExport()
#endif

/// @since 1.0
/// @brief The type of an "PostCreateTypeCallback" callback function.
/// Such a function may be supplied for a Shizu.Type object (at its creation).
/// It is then invoked
/// - multiple times until it succeeds for the first time or the type is destroyed
/// (It is considered as a successful call to a "PostCreateTypeCallback" and "PreDestroyTypeCallback" functions if no such function is provided.)
/// @param state1 A pointer to the Shizu_State1 object.
typedef void (Shizu_PostCreateTypeCallback)(Shizu_State1* state1);

/// @since 1.0
/// @brief The type of an "PreDestroyTypeCallback" callback function.
/// Such a function may be supplied for a Shizu.Type object (at its creation).
/// It is then invoked
/// - exactly once before the type is destroyed
/// - if it was preceeded by a successful call to a "PostCreateTypeCallback" function.
/// (It is considered as a successful call to a "PostCreateTypeCallback" and "PreDestroyTypeCallback" functions if no such function is provided.)
/// @param state1 A pointer to the Shizu_State1 object.
typedef void (Shizu_PreDestroyTypeCallback)(Shizu_State1* state1);


/// @since 1.0
/// @brief The type of a "VisitTypeCallback" callback function.
/// @param state1 A pointer to the Shizu_State1 object.
typedef void (Shizu_VisitTypeCallback)(Shizu_State1* state1);

/// @since 1.0
/// @brief The type of a "Construct" callback function.
/// @param state2 A pointer to the Shizu_State2 object.
typedef void (Shizu_OnConstructCallback)(Shizu_State2* state2, Shizu_Value* returnValue, Shizu_Integer32 numberOfArgumentValues, Shizu_Value* argumentValues);

/// @since 1.0
/// The type of a "onVisit" callback function.
typedef void (Shizu_OnVisitCallback)(Shizu_State2* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onFinalize" callback function.
typedef void (Shizu_OnFinalizeCallback)(Shizu_State2* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onTypeDestroyed" callback function.
typedef void (Shizu_OnTypeDestroyedCallback)(Shizu_State1* state);

/// @since 1.0
/// The type of a "onDispatchInitialize" callback function.
typedef void (Shizu_OnDispatchInitializeCallback)(Shizu_State1* state1, void*);

/// @since 1.0
/// The type of a "onStaticUninitialize" callback function.
typedef void (Shizu_OnDispatchUninitializeCallback)(Shizu_State1* state1, void*);

struct Shizu_ObjectTypeDescriptor {
  Shizu_PostCreateTypeCallback* postCreateType;
  Shizu_PreDestroyTypeCallback* preDestroyType;
  Shizu_VisitTypeCallback* visitType;

  size_t size;
  Shizu_OnConstructCallback* construct;
  Shizu_OnVisitCallback* visit;
  Shizu_OnFinalizeCallback* finalize;

  size_t dispatchSize;
  Shizu_OnDispatchInitializeCallback *dispatchInitialize;
  Shizu_OnDispatchUninitializeCallback* dispatchUninitialize;
};

struct Shizu_PrimitiveTypeDescriptor {
  Shizu_PostCreateTypeCallback* postCreateType;
  Shizu_PreDestroyTypeCallback* preDestroyType;
  Shizu_VisitTypeCallback* visitType;
  size_t size;
};

/**
 * @since 1.0
 * @brief
 * Get if a type is an object type.
 * @param x The type.
 * @return @a true if @a x is an object type. @a false otherwise.
 */
bool
Shizu_Type_isObjectType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/**
 * @since 1.0
 * @brief
 * Get if a type is a primitive type.
 * @param x The type.
 * @return @a true if @a x is a primitive type. @a false otherwise.
 */
bool
Shizu_Type_isPrimitiveType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @brief Get the object type descriptor.
/// @param state1 A pointer to the Shizu_State2 object.
/// @param type A pointer to an object type.
/// @return A pointer to the object type descriptor.
/// @undefined @a state does not point to a Shizu_State2 object.
/// @undefined @a self does not point to a an object type.
Shizu_ObjectTypeDescriptor const*
Shizu_Type_getObjectTypeDescriptor
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @brief Get the primitive type descriptor.
/// @param state1 A pointer to the Shizu_State2 object.
/// @param type A pointer to a primitive type.
/// @return A pointer to the primitive type descriptor.
/// @undefined @a state does not point to a Shizu_State2 object.
/// @undefined @a self does not point to a a primitive type.
Shizu_PrimitiveTypeDescriptor const*
Shizu_Type_getPrimitiveTypeDescriptor
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/**
 * @since 1.0
 * @brief
 * Get if a type is a sub-type of another type.
 * The type-algebraic expression is
 * @code
 * x <= y
 * @endcode
 * @param x The type to test if it is a sub-type of @a y.
 * @param y The type to test if it is a super-type of @a x.
 * @return @a true if @a x is a sub-type of @a y. @a false otherwise.
 */
bool
Shizu_Types_isSubTypeOf
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  );

/**
 * @since 1.0
 * @brief
 * Get if a type is a true sub-type of another type.
 * The type-algebraic expression is
 * @code
 * x <= y
 * @endcode
 * @param x The type to test if it is a true sub-type of @a y.
 * @param y The type to test if it is a true super-type of @a x.
 * @return @a true if @a x is a true sub-type of @a y. @a false otherwise.
 */
bool
Shizu_Types_isTrueSubTypeOf
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  );

/**
 * @since 1.0
 * @brief Get the parent type of a type.
 * @param x The type.
 * @return The parent type if any, null otherwise.
 */
Shizu_Type*
Shizu_Types_getParentType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x
  );

void
Shizu_Types_getTypeName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x,
    char const** bytes,
    size_t* numberOfBytes
  );

/**
 * @since 1.0
 * @brief Get the dispatch of a type.
 * @param x The type.
 * @return The dispatch of a type if any, null otherwise.
 * @remarks This function returns null if the type is not an object type or the dispatch was not yet constructed.
 */
Shizu_Object_Dispatch*
Shizu_Types_getDispatch
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    Shizu_Type* x
  );

/// @brief Macro to declare an enumeration type.
#define Shizu_declareEnumerationType(Name) \
  typedef enum Name Name;

/// @brief Macro the define an enumeration type.
#define Shizu_defineEnumerationType(Name) \
  /* Intentionally empty. */

#define Shizu_declareObjectType(Name) \
  typedef struct Name Name; \
\
  typedef struct Name##_Dispatch Name##_Dispatch; \
\
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State2* state \
    );

// The DL a type is created by must not be unloaded as long as the type exists.
// For a type T defined in a DL we store in T.dl a reference to the DL in the type object.
// If T is defined in the executable we store in T.dl the null reference.
#define Shizu_defineObjectType(Name, ParentName) \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ); \
  \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ) \
  {/*Intentionally empty.*/} \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State2* state \
    ) \
  { \
    Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), #Name, sizeof(#Name) - 1); \
    if (!type) { \
      Shizu_Dl* dl = Shizu_State1_getDlByAdr(Shizu_State2_getState1(state), &Name##_getType); \
      type = Shizu_Types_createObjectType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), #Name, sizeof(#Name) - 1, ParentName##_getType(state), dl, &Name##_typeDestroyed, &Name##_Type); \
      if (dl) { \
        Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl); \
      } \
    } \
    return type; \
  }

/**
 * @since 1.0
 * @brief Get a type by its name.
 * @details Search for a type of the name @code{(bytes, numberOfBytes)}.
 * @param state1 A pointer to a Shizu_State1 object.
 * @param self A pointer to the Shizu_Types object.
 * @param bytes A pointer to an array of @a numberOfBytes Bytes.
 * @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
 * @return A pointer to the type if it was found. The null pointer otherwise.
 */
Shizu_Type*
Shizu_Types_getTypeByName
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes
  );

/// @since 1.0
/// @brief Create an object type.
/// @details
/// Create a type of the name @code{(bytes, numberOfBytes)}.
/// Raise an error if a type of that name already exists.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @param parentType A pointer to the parent type or the null pointer.
/// A pointer to the dynamic library the type descriptor is defined in.
/// The null pointer if the type descriptor is defined in the main program.
/// @param typeDestroyed A pointer to the Shizu_OnTypeDestroyedCallback callback function.
/// @param descriptor A pointer to the type descriptor.
Shizu_Type*
Shizu_Types_createObjectType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_ObjectTypeDescriptor const* typeDescriptor
  );

/// @since .0
/// @brief Create a scalar type.
/// @details
/// Create a type of the name @code{(bytes, numberOfBytes)}.
/// Raise an error if a type of that name already exists.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @param dl
/// A pointer to the dynamic library the type descriptor is defined in.
/// The null pointer if the type descriptor is defined in the main program.
/// @param typeDestroyed A pointer to the Shizu_OnTypeDestroyedCallback callback function.
/// @param descriptor A pointer to the type descriptor.
Shizu_Type*
Shizu_Types_createPrimitiveType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_PrimitiveTypeDescriptor const* typeDescriptor
  );

#endif // SHIZU_RUNTIME_TYPE_H_INCLUDED
