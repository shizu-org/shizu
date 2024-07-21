/*
  Shizu
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

// size_t
#include <stddef.h>
// strlen
#include <string.h>

typedef struct Shizu_State1 Shizu_State1;
typedef struct Shizu_State2 Shizu_State2;
typedef struct Shizu_Dl Shizu_Dl;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Object_Dispatch Shizu_Object_Dispatch;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_ObjectTypeDescriptor Shizu_ObjectTypeDescriptor;
typedef struct Shizu_PrimitiveTypeDescriptor Shizu_PrimitiveTypeDescriptor;
typedef struct Shizu_EnumerationTypeDescriptor Shizu_EnumerationTypeDescriptor;
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

struct Shizu_EnumertionTypeDescriptor {
  Shizu_PostCreateTypeCallback* postCreateType;
  Shizu_PreDestroyTypeCallback* preDestroyType;
  Shizu_VisitTypeCallback* visitType;
  size_t size;
};

/// @since
/// 1.0
/// @remarks
/// Stable interface.
/// @brief
/// Get a type by its name.
/// @details
/// Search for a type of the name @code{(bytes, numberOfBytes)}.
/// @param state
/// A pointer to a Shizu_State1 object.
/// @param self
/// A pointer to the Shizu_Types object.
/// @param bytes
/// A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes
/// The number of Bytes in the array pointed to by @a bytes.
/// @return
/// A pointer to the type if it was found. The null pointer otherwise.
Shizu_Type*
Shizu_Types_getTypeByName
  (
    Shizu_State1* state,
    Shizu_Types* self,
    uint8_t const* bytes,
    Shizu_Integer32 numberOfBytes
  );

/// @since
/// 1.0
/// @remarks
/// Stable interface.
/// @brief
/// Get the name of a type.
/// @param state
/// A pointer to a Shizu_State1 object.
/// @param self
/// A pointer to the Shizu_Types object.
/// @param x
/// A pointer to the type.
/// @param [out] bytes
/// A pointer to a <code>uint8_t*</code> variable.
/// @param [out] numberOfBytes
/// A pointer to a <code>size_t</code> variable.
/// @post
/// <code>*bytes</code> was assigned a pointer to an array of Bytes.
/// <code>*numberOfBytes</code> was assigned the number of Bytes in the array assigned to <code>*bytes</code>.
/// The array is valid for the lifetime of the type object.
void
Shizu_Types_getTypeName
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x,
    uint8_t const** bytes,
    Shizu_Integer32* numberOfBytes
  );

/// @since
/// 1.0
/// @remarks
/// Stable interface.
/// @brief
/// Get the hash value of a type.
/// @param state
/// A pointer to a Shizu_State1 object.
/// @param self
/// A pointer to the Shizu_Types object.
/// @param x
/// A pointer to the type.
/// @return
/// The hash value of the type.
Shizu_Integer32
Shizu_Types_getTypeHash
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x
  );

/// @since 1.0
/// @remarks Stable interface.
/// @brief
/// Get if a type is an object type.
/// @param state
/// A pointer to a Shizu_State1 object.
/// @param self
/// A pointer to the Shizu_Types object.
/// @param x
/// A pointer to the type.
/// @return
/// @a true if @a x is an object type. @a false otherwise.
bool
Shizu_Types_isObjectType
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @since 1.0
/// @remarks Stable interface.
/// @brief
/// Get if a type is a primitive type.
/// @param x The type.
/// @return @a true if @a x is a primitive type. @a false otherwise.
/// @remarks Stable API.
bool
Shizu_Types_isPrimitiveType
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @since 1.0
/// @remarks Stable interface.
/// @brief
/// Get if a type is an enumeration type.
/// @param x The type.
/// @return @a true if @a x is an enumeration type. @a false otherwise.
bool
Shizu_Types_isEnumerationType
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @since 1.0
/// @remarks Stable interface.
/// @brief
/// Get if a type is a sub-type of another type.
/// The type-algebraic expression is
/// @code
/// x <= y
/// @endcode
/// @param x The type to test if it is a sub-type of @a y.
/// @param y The type to test if it is a super-type of @a x.
/// @return @a true if @a x is a sub-type of @a y. @a false otherwise.
bool
Shizu_Types_isSubTypeOf
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  );

/// @since 1.0
/// @remarks Stable interface.
/// @brief
/// Get if a type is a true sub-type of another type.
/// The type-algebraic expression is
/// @code
/// x <= y
/// @endcode
/// @param x The type to test if it is a true sub-type of @a y.
/// @param y The type to test if it is a true super-type of @a x.
/// @return @a true if @a x is a true sub-type of @a y. @a false otherwise.
bool
Shizu_Types_isTrueSubTypeOf
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x,
    Shizu_Type const* y
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
    Shizu_State1* state,
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
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type const* x
  );

/// @since 1.0
/// @brief Get the parent type of a type.
/// @param x The type.
/// @return The parent type if any, null otherwise.
Shizu_Type*
Shizu_Types_getParentType
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x
  );

/// @since 1.0
/// @brief Get the dispatch of a type.
/// @param x The type.
/// @return The dispatch of a type if any, null otherwise.
/// @remarks This function returns null if the type is not an object type or the dispatch was not yet constructed.
Shizu_Object_Dispatch*
Shizu_Types_getDispatch
  (
    Shizu_State1* state,
    Shizu_Types* self,
    Shizu_Type* x
  );

/// @brief Macro to declare an enumeration type.
/// @param CxxName The name of the type in the CXX domain. For example "My_Strategy".
#define Shizu_declareEnumerationType(CxxName) \
  typedef enum CxxName CxxName;

/// @brief Macro the define an enumeration type.
/// @param MlName The name of the type in the ML domain. For example "My.Strategy".
/// @param CxxName The name of the type in the CXX domain. For example "My_Strategy".
#define Shizu_defineEnumerationType(MlName, CxxName) \
  /* Intentionally empty. */

/// @brief Macro to declare an object type.
/// @param CxxName The name of the type in the CXX domain. For example "My_Device".
#define Shizu_declareObjectType(CxxName) \
  typedef struct CxxName CxxName; \
\
  typedef struct CxxName##_Dispatch CxxName##_Dispatch; \
\
  Shizu_Type* \
  CxxName##_getType \
    ( \
      Shizu_State2* state \
    );
 
/// @param MlName The name of the type in the ML domain. For example "My.Device".
/// @param CxxName The name of the type in the CXX domain. For example "My_Device".
/// @remarks
/// The DL a type is created by must not be unloaded as long as the type exists.
/// For a type T defined in a DL we store in T.dl a reference to the DL in the type object.
/// If T is defined in the executable we store in T.dl the null reference.
#define Shizu_defineObjectType(MlName, CxxName, ParentName) \
  static void \
  CxxName##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ); \
  \
  static void \
  CxxName##_typeDestroyed \
    ( \
      Shizu_State1* state1 \
    ) \
  {/*Intentionally empty.*/} \
  \
  Shizu_Type* \
  CxxName##_getType \
    ( \
      Shizu_State2* state \
    ) \
  { \
    size_t n = strlen(MlName); \
    if (n > Shizu_Integer32_Maximum) { \
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid); \
      Shizu_State2_jump(state); \
    } \
    Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), MlName, (Shizu_Integer32)n); \
    if (!type) { \
      Shizu_Dl* dl = Shizu_State1_getDlByAdr(Shizu_State2_getState1(state), &CxxName##_getType); \
      type = Shizu_Types_createObjectType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), MlName, (Shizu_Integer32)n, ParentName##_getType(state), dl, &CxxName##_typeDestroyed, &CxxName##_Type); \
      if (dl) { \
        Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl); \
      } \
    } \
    return type; \
  }

#define Shizu_declarePrimitiveType(Name) \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State2* state \
    ); 

/// @remarks
/// The DL a type is created by must not be unloaded as long as the type exists.
/// For a type T defined in a DL we store in T.dl a reference to the DL in the type object.
/// If T is defined in the executable we store in T.dl the null reference.
#define Shizu_definePrimitiveType(Name) \
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
      type = Shizu_Types_createPrimitiveType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), #Name, sizeof(#Name) - 1, dl, &Name##_typeDestroyed, &Name##_Type); \
      if (dl) { \
        Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl); \
      } \
    } \
    return type; \
  }

/// @since 1.0
/// @brief Create an object type.
/// @details
/// Create a type of the name @code{(bytes, numberOfBytes)}.
/// Raise an error if a type of that name already exists.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @param parentType A pointer to the parent type or the null pointer.
/// @param dl A pointer to the dynamic library the type descriptor is defined in.
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

/// @since 1.0
/// @brief Create an enumeration type.
/// @details
/// Create a type of the name @code{bytes, numberOfBytes)}.
/// Raise an error if a type of that name already exists.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @param dl A pointer to the dynamic library the type descriptor is defined in.
/// The null pointer if the type descriptor is defined in the main program.
/// @param typeDestroyed A pointer to the Shizu_OnTypeDestroyedCallback callback function.
/// @param descriptor A pointer to the type descriptor.
Shizu_Type*
Shizu_Types_createEnumerationType
  (
    Shizu_State1* state1,
    Shizu_Types* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_EnumerationTypeDescriptor const* typeDescriptor
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



Shizu_declarePrimitiveType(Shizu_Boolean);



Shizu_declarePrimitiveType(Shizu_CxxFunction);



Shizu_declarePrimitiveType(Shizu_Float32);

/// A non-negative value to subtract from the binary value of the biased exponent to receive the unbiased exponent.
/// This value is 127.
Shizu_Integer32
Shizu_Float32_getBias
  (
    Shizu_State1* state
  );

// This value is biasedMinimumExponent - bias = 1 - 127 = -126.
Shizu_Integer32
Shizu_Float32_getUnbiasedMinimumExponent
(
  Shizu_State1* state
);

// This value is biasedMaximumExponent - bias = 254 - 127 = 127.
Shizu_Integer32
Shizu_Float32_getUnbiasedMaximumExponent
  (
    Shizu_State1* state
  );

// This value is 1 (as 0 and 2^8-1 = 255 are reserved).
// The "biased exponent" is the "unbiased exponent" plus some non-negative "bias"
// that ensures that the "biased exponent" is always non-negative.
Shizu_Integer32
Shizu_Float32_getBiasedMinimumExponent
  (
    Shizu_State1* state
  );

// This value is 254 (as 0 and 2^8 - 1 = 255 are reserved).
// The "biased exponent" is the "unbiased exponent" plus some non-negative "bias"
// that ensures that the "biased exponent" is always non-negative.
Shizu_Integer32
Shizu_Float32_getBiasedMaximumExponent
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getExponentBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getExponentBitsShift
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getSignBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getSignBitsShift
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getFractionBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer32
Shizu_Float32_getFractionBitsShift
  (
    Shizu_State1* state
  );



#if 1 == Shizu_Configuration_WithFloat64

Shizu_declarePrimitiveType(Shizu_Float64);

/// A non-negative value to subtract from the binary value of the biased exponent to receive the unbiased exponent.
/// This value is 1023.
Shizu_Integer64
Shizu_Float64_getBias
  (
    Shizu_State1* state
  );

// This value is biasedMinimumExponent - bias = 1 - 1023 = -1022.
Shizu_Integer64
Shizu_Float64_getUnbiasedMinimumExponent
  (
    Shizu_State1* state
  );

// This value is biasedMaximumExponent - bias = 2046 - 1023 = 1023.
Shizu_Integer64
Shizu_Float64_getUnbiasedMaximumExponent
  (
    Shizu_State1* state
  );

// This value is 1 (as 0 and 2^11 - 1 = 2047 are reserved).
// The "biased exponent" is the "unbiased exponent" plus some non-negative "bias"
// that ensures that the "biased exponent" is always non-negative.
Shizu_Integer64
Shizu_Float64_getBiasedMinimumExponent
  (
    Shizu_State1* state
  );

// This value is 2046 (as 0 and 2^11 - 1 = 2047 are reserved).
// The "biased exponent" is the "unbiased exponent" plus some non-negative "bias"
// that ensures that the "biased exponent" is always non-negative.
Shizu_Integer64
Shizu_Float64_getBiasedMaximumExponent
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getExponentBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getExponentBitsShift
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getSignBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getSignBitsShift
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getFractionBitsMask
  (
    Shizu_State1* state
  );

Shizu_Integer64
Shizu_Float64_getFractionBitsShift
  (
    Shizu_State1* state
  );

#endif



Shizu_declarePrimitiveType(Shizu_Integer32);



#if 1 == Shizu_Configuration_WithInteger64

Shizu_declarePrimitiveType(Shizu_Integer64);

#endif



Shizu_declarePrimitiveType(Shizu_Type);



Shizu_declarePrimitiveType(Shizu_Void);

#endif // SHIZU_RUNTIME_TYPE_H_INCLUDED
