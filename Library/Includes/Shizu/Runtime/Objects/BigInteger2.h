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

#if !defined(SHIZU_OBJECTS_BIGINTEGER2_H_INCLUDED)
#define SHIZU_OBJECTS_BIGINTEGER2_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
#error("Do not include `Shizu/Runtime/Objects/BigInteger10.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"
typedef struct Shizu_String Shizu_String;

/// @brief A big integer implementation.
/// @detail
/// sign magnitude representation. The magnitude is an array of uint32_t values of length n > 0.
/// Each uint32_t value is a base UINT32_MAX digit (that is, is within the range of [0,UINT32_MAX)).
/// The most significant digit is at index 0 in the array, the least significant digit is at index n - 1.
/// Leading zeroes are not allowed. 
/// @detail
/// The Machine Language type is
/// @code
/// class Shizu.BigInteger2
/// @endcode
/// Its constructor is
/// @code
/// Shizu.BigInteger2.construct()
/// @endcode
Shizu_declareObjectType(Shizu_BigInteger2);

struct Shizu_BigInteger2 {
  Shizu_Object _parent;
  /// Pointer to an array of uint32_t values.
  uint32_t* p;
  /// The length of the array pointed to by @a p.
  uint32_t n;
  /// The sign. Shizu_Boolean_True means plus and Shizu_Boolean_False means minus.
  /// If the value is zero, then the sign must be Shizu_Boolean_True.
  Shizu_Boolean sign;
};

struct Shizu_BigInteger2_Dispatch {
  Shizu_Object_Dispatch _parent;
};

/// @brief Compare this big integer with another big integer.
/// @return
/// - 0 if both are equal.
/// - -1 if this big integer is smaller than the other big integer.
/// - +1 if this big integer is greater than the other big integer.
Shizu_Integer32
Shizu_BigInteger2_compare
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  );

/// @brief Compare this big integer with a 32 bit two's complement integer.
/// @return
/// - 0 if both are equal.
/// - -1 if this big integer is smaller than the 32 bit two's complement integer.
/// - +1 if this big integer is greater than the 32 bit two's complement integer.
Shizu_Integer32
Shizu_BigInteger2_compareInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer32 other
  );

#if 1 == Shizu_Configuration_WithInteger64

/// @brief Compare this big integer with a 64 bit two's complement integer.
/// @return
/// - 0 if both are equal.
/// - -1 if this big integer is smaller than the 64 bit two's complement integer.
/// - +1 if this big integer is greater than the 64 bit two's complement integer.
Shizu_Integer32
Shizu_BigInteger2_compareInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer64 other
  );

#endif

Shizu_Boolean
Shizu_BigInteger2_isZero
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

Shizu_BigInteger2*
Shizu_BigInteger2_negate
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

/// @pre exponent >= 0
Shizu_BigInteger2*
Shizu_BigInteger2_pow
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer32 exponent
  );

Shizu_BigInteger2*
Shizu_BigInteger2_create
  (
    Shizu_State2* state
  );

Shizu_BigInteger2*
Shizu_BigInteger2_createFromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v
  );

#if 1 == Shizu_Configuration_WithInteger64

Shizu_BigInteger2*
Shizu_BigInteger2_createFromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v
  );

#endif

Shizu_BigInteger2*
Shizu_BigInteger2_createFromString
  (
    Shizu_State2* state,
    Shizu_String* v
  );

Shizu_BigInteger2*
Shizu_BigInteger2_createFromBigInteger2
  (
    Shizu_State2* state,
    Shizu_BigInteger2* v
  );

Shizu_String*
Shizu_BigInteger2_toString
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

Shizu_Integer32
Shizu_BigInteger2_toInteger32
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer64
Shizu_BigInteger2_toInteger64
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self
  );

#endif

#if 0
Shizu_BigInteger2*
Shizu_BigInteger2_add
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  );

Shizu_BigInteger2*
Shizu_BigInteger2_subtract
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_BigInteger2* other
  );
#endif

/// @code
/// 0
/// @endcode
Shizu_BigInteger2*
Shizu_BigInteger2_zero
  (
    Shizu_State2* state
  );

/// @code
/// 1
/// @endcode
Shizu_BigInteger2*
Shizu_BigInteger2_one
  (
    Shizu_State2* state
  );

#if 0
/// @code
/// self * 2^x, x >= 0
/// @endcode
Shizu_BigInteger2*
Shizu_BigInteger2_multiply2
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer32 x
  );

/// @code
/// floor(self / 2^x), x >= 0
/// @endcode
Shizu_BigInteger2*
Shizu_BigInteger2_divide10
  (
    Shizu_State2* state,
    Shizu_BigInteger2* self,
    Shizu_Integer32 x
  );
#endif

#endif // SHIZU_OBJECTS_BIGINTEGER2_H_INCLUDED
