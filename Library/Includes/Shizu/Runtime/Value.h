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

#if !defined(SHIZU_RUNTIME_VALUE_H_INCLUDED)
#define SHIZU_RUNTIME_VALUE_H_INCLUDED



#include "Shizu/Runtime/Configure.h"

// int32_t, INT32_MIN, INT32_MAX
#include <inttypes.h>

// bool, true, false
#include <stdbool.h>

// FLT_MAX
#include <float.h>

// Forward declaration.
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_State2 Shizu_State2;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_Value Shizu_Value;



typedef bool Shizu_Boolean;

#define Shizu_Boolean_True (true)

#define Shizu_Boolean_False (false)



typedef float Shizu_Float32;

#define Shizu_Float32_Minimum (-FLT_MAX)

#define Shizu_Float32_Maximum (+FLT_MAX)

// See https://en.wikipedia.org/wiki/Single-precision_floating-point_format for more information.
#define Shizu_Float32_Bits 32
#define Shizu_Float32_SignBits 1
#define Shizu_Float32_FractionBits 23
#define Shizu_Float32_ExponentBits 8
static_assert(Shizu_Float32_SignBits + Shizu_Float32_FractionBits + Shizu_Float32_ExponentBits == 32, "single precision floating point defines broken");



#if 1 == Shizu_Configuration_WithFloat64

typedef double Shizu_Float64;

#define Shizu_Float64_Minimum (-DBL_MAX)

#define Shizu_Float64_Maximum (+DBL_MAX)

// See https://en.wikipedia.org/wiki/Double-precision_floating-point_format for more information.
#define Shizu_Float64_Bits 64
#define Shizu_Float64_SignBits 1
#define Shizu_Float64_FractionBits 52
#define Shizu_Float64_ExponentBits 11
static_assert(Shizu_Float64_SignBits + Shizu_Float64_FractionBits + Shizu_Float64_ExponentBits == 64, "double precision floating point defines broken");

#endif



typedef int32_t Shizu_Integer32;

#define Shizu_Integer32_Minimum (INT32_MIN)

#define Shizu_Integer32_Maximum (INT32_MAX)



#if 1 == Shizu_Configuration_WithInteger64

typedef int64_t Shizu_Integer64;

#define Shizu_Integer64_Minimum (INT64_MIN)

#define Shizu_Integer64_Maximum (INT64_MAX)

#endif



typedef uint8_t Shizu_Void;

#define Shizu_Void_Void (0)



typedef void (Shizu_CxxFunction)(Shizu_State2* state, Shizu_Value* returnValue, Shizu_Integer32 numberOfArgumentValues, Shizu_Value* argumentValues);



typedef struct Shizu_Value Shizu_Value;

#define Shizu_Value_Tag_Boolean (0)

#define Shizu_Value_Tag_CxxFunction (Shizu_Value_Tag_Boolean + 1)

#define Shizu_Value_Tag_Float32 (Shizu_Value_Tag_CxxFunction + 1)

#if 1 == Shizu_Configuration_WithFloat64

#define Shizu_Value_Tag_Float64 (Shizu_Value_Tag_Float32 + 1)

#define Shizu_Value_Tag_Integer32 (Shizu_Value_Tag_Float64 + 1)

#else

#define Shizu_Value_Tag_Integer32 (Shizu_Value_Tag_Float32 + 1)

#endif

#if 1 == Shizu_Configuration_WithInteger64

#define Shizu_Value_Tag_Integer64 (Shizu_Value_Tag_Integer32 + 1)

#define Shizu_Value_Tag_Object (Shizu_Value_Tag_Integer64 + 1)

#else

#define Shizu_Value_Tag_Object (Shizu_Value_Tag_Integer32 + 1)

#endif

#define Shizu_Value_Tag_Type (Shizu_Value_Tag_Object + 1)

#define Shizu_Value_Tag_Void (Shizu_Value_Tag_Type + 1)



struct Shizu_Value {
  uint8_t tag;
  union {
    Shizu_Boolean booleanValue;
    Shizu_CxxFunction* cxxFunctionValue;
    Shizu_Float32 float32Value;
  #if 1 == Shizu_Configuration_WithFloat64
    Shizu_Float64 float64Value;
  #endif
    Shizu_Integer32 integer32Value;
  #if 1 == Shizu_Configuration_WithInteger64
    Shizu_Integer64 integer64Value;
  #endif
    Shizu_Object* objectValue;
    Shizu_Type* typeValue;
    Shizu_Void voidValue;
  };
};



#define Shizu_Value_InitializerBoolean(VALUE) { .tag = Shizu_Value_Tag_Boolean, .booleanValue = (VALUE) }

Shizu_Boolean
Shizu_Value_getBoolean
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isBoolean
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setBoolean
  (
    Shizu_Value* self,
    Shizu_Boolean booleanValue
  );



#define Shizu_Value_InitializerCxxFunction(VALUE) { .tag = Shizu_Value_Tag_CxxFunction, .cxxFunctionValue = (VALUE) }

Shizu_CxxFunction*
Shizu_Value_getCxxFunction
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isCxxFunction
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setCxxFunction
  (
    Shizu_Value* self,
    Shizu_CxxFunction* cxxFunctionValue
  );

#define Shizu_Value_InitializerFloat32(VALUE) { .tag = Shizu_Value_Tag_Float32, .float32Value = (VALUE) }



Shizu_Float32
Shizu_Value_getFloat32
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isFloat32
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setFloat32
  (
    Shizu_Value* self,
    Shizu_Float32 float32Value
  );



#if 1 == Shizu_Configuration_WithFloat64

#define Shizu_Value_InitializerFloat64(VALUE) { .tag = Shizu_Value_Tag_Float64, .floa64Value = (VALUE) }

Shizu_Float64
Shizu_Value_getFloat64
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isFloat64
(
  Shizu_Value const* self
);

void
Shizu_Value_setFloat64
(
  Shizu_Value* self,
  Shizu_Float64 float64Value
);

#endif



#define Shizu_Value_InitializerInteger32(VALUE) { .tag = Shizu_Value_Tag_Integer32, .integer32Value = (VALUE) }

Shizu_Integer32
Shizu_Value_getInteger32
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isInteger32
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setInteger32
  (
    Shizu_Value* self,
    Shizu_Integer32 integer32Value
  );



#if 1 == Shizu_Configuration_WithInteger64

#define Shizu_Value_InitializerInteger64(VALUE) { .tag = Shizu_Value_Tag_Integer64, .integer64Value = (VALUE) }

Shizu_Integer64
Shizu_Value_getInteger64
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isInteger64
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setInteger64
  (
    Shizu_Value* self,
    Shizu_Integer64 integer64Value
  );

#endif



#define Shizu_Value_InitializerObject(VALUE) { .tag = Shizu_Value_Tag_Object, .objectValue = (Shizu_Object*)(VALUE) }

Shizu_Object*
Shizu_Value_getObject
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isObject
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setObject
  (
    Shizu_Value* self,
    Shizu_Object* objectValue
  );



#define Shizu_Value_InitializerType(VALUE) { .tag = Shizu_Value_Tag_Type, .typeValue = (VALUE) }

void
Shizu_Value_setType
  (
    Shizu_Value* self,
    Shizu_Type* typeValue
  );

Shizu_Type*
Shizu_Value_getType
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isType
  (
    Shizu_Value const* self
  );



#define Shizu_Value_InitializerVoid(VALUE) { .tag = Shizu_Value_Tag_Void, .voidValue = (VALUE) }

Shizu_Void
Shizu_Value_getVoid
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isVoid
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setVoid
  (
    Shizu_Value* self,
    Shizu_Void voidValue
  );



Shizu_Boolean
Shizu_Value_isEqualTo
  (
    Shizu_State2* state,
    Shizu_Value const* x,
    Shizu_Value const* y
  );

Shizu_Integer32
Shizu_Value_getHashValue
  (
    Shizu_State2* state,
    Shizu_Value const* x
  );

#endif // SHIZU_RUNTIME_VALUE_H_INCLUDED
