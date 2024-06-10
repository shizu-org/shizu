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

// bool
#include <stdbool.h>

typedef struct Shizu_State1 Shizu_State1;
typedef struct Shizu_Dl Shizu_Dl;
typedef struct Shizu_State Shizu_State;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Object_Dispatch Shizu_Object_Dispatch;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_TypeDescriptor Shizu_TypeDescriptor;
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
/// The type of a "onVisit" callback function.
typedef void (Shizu_OnVisitCallback)(Shizu_State* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onFinalize" callback function.
typedef void (Shizu_OnFinalizeCallback)(Shizu_State* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onTypeDestroyed" callback function.
typedef void (Shizu_OnTypeDestroyedCallback)(Shizu_State1* state);

/// @since 1.0
/// The type of a "onDispatchInitialize" callback function.
typedef void (Shizu_OnDispatchInitializeCallback)(Shizu_State1* state1, void*);

/// @since 1.0
/// The type of a "onStaticUninitialize" callback function.
typedef void (Shizu_OnDispatchUninitializeCallback)(Shizu_State1* state1, void*);

struct Shizu_TypeDescriptor {
  Shizu_PostCreateTypeCallback* postCreateType;
  Shizu_PreDestroyTypeCallback* preDestroyType;
  Shizu_VisitTypeCallback* visitType;
  size_t size;
  Shizu_OnVisitCallback* visit;
  Shizu_OnFinalizeCallback* finalize;
  size_t dispatchSize;
  Shizu_OnDispatchInitializeCallback *dispatchInitialize;
  Shizu_OnDispatchUninitializeCallback* dispatchUninitialize;
};

/**
 * @since 1.0
 * @brief Get if a type is a sub-type of another type.
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
 * @brief Get if a type is a true sub-type of another type.
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

#define Shizu_declareType(Name) \
  typedef struct Name Name; \
\
  typedef struct Name##_Dispatch Name##_Dispatch; \
\
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    );

// The DL a type is created by must not be unloaded as long as the type exists.
// For a type T defined in a DL we store in T.dl a reference to the DL in the type object.
// If T is defined in the executable we store in T.dl the null reference.
#define Shizu_defineType(Name, ParentName) \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ); \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
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
      Shizu_State* state \
    ) \
  { \
    Shizu_Type* type = Shizu_State_getTypeByName(state, #Name, sizeof(#Name) - 1); \
    if (!type) { \
      Shizu_Dl* dl = Shizu_State_getDlByAdr(state, &Name##_getType); \
      type = Shizu_State_createType(state, #Name, sizeof(#Name) - 1, ParentName##_getType(state), dl, &Name##_typeDestroyed, &Name##_Type); \
    } \
    return type; \
  }

#endif // SHIZU_RUNTIME_TYPE_H_INCLUDED
