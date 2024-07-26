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
#include "Shizu/Cxx/Include.h"

// int32_t, INT32_MIN, INT32_MAX
#include <inttypes.h>

// Minimum number of decimal digits required to represent any int64_t value.
// INT64_MAX i s +9,223,372,036,854,775,807 and the minimum value is -9,223,372,036,854,775,808.
#define INT64_DECIMAL_DIG (19)

// Minimum number of decimal digits required to represent any int32_t value.
// INT32_MAX is +2,147,483,647, INT32_MIN is -2,147,483,648. Hence 10 digits are required. 
#define INT32_DECIMAL_DIG (10)

// Minimal number of decimal digits required to represent any uint64_t value.
// UINT64_MAX is 18,446,744,073,709,551,615. Hence 20 decimal digits are required.
#define UINT64_DECIMAL_DIG (20)

// Minimal number of decimal digits required to represent any uint32_t value.
// UINT32_MAX is 4,294,967,295. Hence 10 digits are required.
#define UINT32_DECIMAL_DIG (10)

// Minimal number of decimal digits required to represent any uint16_t value.
// UINT16_MAX is 65,535. Hence 5 digits are required.
#define UINT16_DECIMAL_DIG (5)

// Minimal number of decimal digits required to represent any uint8_t value.
// UINT8_MAX is 255. Hence 3 digits are required.
#define UINT8_DECIMAL_DIG (3)

// Greatest power of 10 an uint64_t can hold.
#define UINT64_MAX_POW10 10000000000000000000
static_assert(UINT64_MAX_POW10 <= UINT64_MAX, "<internal error>"); // UINT64_MAX is 18.446.744.073.709.551.615

// Greatest power of 10 an uint32_t can hold.
#define UINT32_MAX_POW10 1000000000
static_assert(UINT32_MAX_POW10 <= UINT32_MAX, "<internal error>"); // UINT32_MAX is 4.294.967.295

// Greatest power of 10 an uin16_t can hold.
#define UINT16_MAX_POW10 10000
static_assert(UINT16_MAX_POW10 <= UINT16_MAX, "<internal error>"); // UINT16_MAX is 65.535

// Greatest power of 10 an uint8_t can hold.
#define UINT8_MAX_POW10 100
static_assert(UINT8_MAX_POW10 <= UINT8_MAX, "<internal error>");   // UINT8_MAX is 255

// bool, true, false
#include <stdbool.h>

// FLT_MAX
#include <float.h>

// HUGE_VALF, HUGE_VAL
#include <math.h>

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

#define Shizu_Float32_Infinity ((float)HUGE_VALF)

/// The minimum of the actual exponent value range.
/// The exponent is an 8 bit field (Shizu_Float32_ExponentBits) which encodes value [0,+255].
/// The values 0 and 255 never represent an actual exponent value.
/// Hence the values [+1,+254] remain.
/// Add a bias of -127 to these values to obtain the actual exponent value range [-126,+127].
/// 
/// Note that FLT_MIN_EXP evaluates to -125 (that is, one more than this constant).
/// Quote: "minimum negative integer such that FLT_RADIX raised by power one less than that integer is a normalized float"
#define Shizu_Float32_MinimumExponent (-126) /*FLT_MIN_EXP-1*/
/// The maximum of the actual exponent value range.
///
/// Note that FLT_MAX_EXP evaluates to +128 (that is one more than this constant).
/// Quote: "maximum positive integer such that FLT_RADIX raised by power one less than that integer is a representable finite float"
#define Shizu_Float32_MaximumExponent (+127) /*FLT_MAX_EXP - 1*/

/// Note that this constant and FLT_MIN_10_EXP both are defined as -37.
/// Note: This is something like floor(log10(2^Shizu_Float32_MinimumExponent)) = floor(log10(2^-126)).
#define Shizu_Float32_MinimumDecimalExponent (-37)
/// Note that this constant and FLT_MAX_10_EXP both are defined as 38.
/// Note: This is something like floor(log10(2^Shizu_Float32_MaximumExponent)) = floor(log10(2^127)).
#define Shizu_Float32_MaximumDecimalExponent (38)

#define Shizu_Float32_ExponentBias (127)

/// The maximum number of digits of the significand that are considered when converting from decimal to binary.
#define Shizu_Float32_MaximumInputDecimalDigits (200)

// See https://en.wikipedia.org/wiki/Single-precision_floating-point_format for more information.
#define Shizu_Float32_Bits 32

/// The number sign bits.
#define Shizu_Float32_SignBits 1
/// The shift of the sign bits.
#define Shizu_Float32_SignShift (Shizu_Float32_ExponentBits + Shizu_Float32_SignificandBits)
/// Mask for the sign bits.
#define Shizu_Float32_SignMask (((1 << Shizu_Float32_SignBits) - 1) << Shizu_Float32_SignShift)

/// The number of stored significand bits (this is 23).
#define Shizu_Float32_SignificandBits 23
/// The shift of the significand bits.
#define Shizu_Float32_SignificandShift (0)
/// Mask for the significand bits.
#define Shizu_Float32_SignificandMask (((1 << Shizu_Float32_SignificandBits) - 1) << Shizu_Float32_SignificandShift)

/// The number of exponent bits.
#define Shizu_Float32_ExponentBits 8
/// The shift of the exponent bits.
#define Shizu_Float32_ExponentShift (Shizu_Float32_SignificandBits)
/// Mask for the exponent bits.
#define Shizu_Float32_ExponentMask (((1 << Shizu_Float32_ExponentBits) - 1) << Shizu_Float32_ExponentShift) 


static_assert(Shizu_Float32_SignBits + Shizu_Float32_SignificandBits + Shizu_Float32_ExponentBits == 32, "single precision floating point defines broken");
/// The "precision" is the number of explicitly stored significand bits plus the implicit leading 1 bit.
#define Shizu_Float32_Precision (Shizu_Float32_SignificandBits + 1)


#if 1 == Shizu_Configuration_WithFloat64

typedef double Shizu_Float64;

#define Shizu_Float64_Minimum (-DBL_MAX)

#define Shizu_Float64_Maximum (+DBL_MAX)

#define Shizu_Float64_Infinity ((double)HUGE_VAL)

/// The maximum of the actual exponent value range.
/// The exponent is an 11 bit field (Shizu_Float64_ExponentBits) which encodes value [0,+2047].
/// The values 0 and 2047 never represent an actual exponent value.
/// Hence the values [+1,+2046] remain.
/// Add a bias of -1023 to these values to obtain the actual exponent value range [-1022,+1023].
/// 
/// Note that DBL_MIN_EXP evaluates to -1021 (that is, one more than this constant).
/// Quote: "minimum negative integer such that DBL_RADIX raised by power one less than that integer is a normalized double"
#define Shizu_Float64_MinimumExponent (-1022) /*DBL_MIN_EXP + 1*/

/// The minimum of the actual exponent value range.
///
/// Note that DBL_MAX_EXP evaluates to +1024 (that is one more than this constant).
/// Quote: "maximum positive integer such that DBL_RADIX raised by power one less than that integer is a representable finite float"
#define Shizu_Float64_MaximumExponent (+1023) /*DBL_MAX_EXP - 1*/

/// Note that both DBL_MIN_10_EXP and this constant are defined as 307.
#define Shizu_Float64_MinimumDecimalExponent (-307)
/// Note that both DBL_MAX_10_EXP and this constant are defined as 308.
#define Shizu_Float64_MaximumDecimalExponent (308)

#define Shizu_Float64_ExponentBias (1023)

/// The maximum number of digits of the significand that are considered when converting from decimal to binary.
#define Shizu_Float64_MaximumInputDecimalDigits (1100)

// See https://en.wikipedia.org/wiki/Double-precision_floating-point_format for more information.
#define Shizu_Float64_Bits 64

/// The number of sign bits.
#define Shizu_Float64_SignBits 1
/// The shift of the sign bits.
#define Shizu_Float64_SignShift (Shizu_Float64_ExponentBits + Shizu_Float64_SignificandBits)
/// Mask for the sign bits.
#define Shizu_Float64_SignMask (((1 << Shizu_Float64_SignBits) - 1) << Shizu_Float64_SignShift)

/// The number of significand bits.
#define Shizu_Float64_SignificandBits 52
/// The shift of the significand bits.
#define Shizu_Float64_SignificandShift (0)
/// Mask for the significand bits.
#define Shizu_Float64_SignificandMask (((1 << Shizu_Float64_SignificandBits) - 1) << Shizu_Float64_SignificandShift)

/// The number of exponent bits.
#define Shizu_Float64_ExponentBits 11
/// The shift of the exponent bits.
#define Shizu_Float64_ExponentShift (Shizu_Float64_SignificandBits)
/// Mask for the exponent bits.
#define Shizu_Float64_ExponentMask (((1 << Shizu_Float64_ExponentBits) - 1) << Shizu_Float64_ExponentShift)

static_assert(Shizu_Float64_SignBits + Shizu_Float64_SignificandBits + Shizu_Float64_ExponentBits == 64, "double precision floating point defines broken");
/// The "precision" is the number of explicitly stored significand bits plus the implicit leading 1 bit.
#define Shizu_Float64_Precision (Shizu_Float64_SignificandBits + 1)

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
