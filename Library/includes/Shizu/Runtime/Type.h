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

#include "Shizu/Runtime/State1.h"

typedef struct Shizu_Dl Shizu_Dl;
typedef struct Shizu_State Shizu_State;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_TypeDescriptor Shizu_TypeDescriptor;
typedef struct Shizu_Types Shizu_Types;

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_DlExport() __declspec(dllexport)
#else
  #define Shizu_DlExport()
#endif

/// @since 1.0
/// The type of an "onStaticInitialize" callback function.
typedef void (Shizu_OnStaticInitializeCallback)(Shizu_State* state);

/// @since 1.0
/// The type of an "onStaticUninitialize" callback function.
typedef void (Shizu_OnStaticFinalizeCallback)(Shizu_State* state);

/// @since 1.0
/// The type of an "onStaticVisit" callback function.
typedef void (Shizu_OnStaticVisitCallback)(Shizu_State* state);

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
  Shizu_OnStaticInitializeCallback* staticInitialize;
  Shizu_OnStaticFinalizeCallback* staticFinalize;
  Shizu_OnStaticVisitCallback* staticVisit;
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
Shizu_State_isSubTypeOf
  (
    Shizu_State* self,
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
Shizu_Type_isTrueSubTypeOf
  (
    Shizu_State* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  );

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* state,
    char const* name
  );

Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* descriptor
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
// If T is defined in the  executable we store in T.dl the null reference.
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
    Shizu_Type* type = Shizu_State_getTypeByName(state, #Name); \
    if (!type) { \
      Shizu_Dl* dl = Shizu_State_getDlByAdr(state, &Name##_getType); \
      type = Shizu_State_createType(state, #Name, ParentName##_getType(state), dl, &Name##_typeDestroyed, &Name##_Type); \
    } \
    return type; \
  }

#endif // SHIZU_RUNTIME_TYPE_H_INCLUDED
