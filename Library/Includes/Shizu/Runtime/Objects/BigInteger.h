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

#if !defined(SHIZU_RUNTIME_OBJECTS_BIGINTEGER_H_INCLUDED)
#define SHIZU_RUNTIME_OBJECTS_BIGINTEGER_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
#error("Do not include `Shizu/Runtime/Objects/BigInteger.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"
typedef struct Shizu_String Shizu_String;

/// @brief A big integer implementation.
/// @detail
/// The Machine Language type is
/// @code
/// class Shizu.BigInteger
/// @endcode
/// Its constructor is
/// @code
/// Shizu.BigInteger.construct()
/// @endcode
Shizu_declareObjectType(Shizu_BigInteger);

struct Shizu_BigInteger {
  Shizu_Object _parent;
  // Pointer to an array of uint8_t values.
  // Each value is within the bounds of [0,9] and represents a decimal digit.
  // The digit at index 0 is the most significand, the digit at index n-1 (where n is the size of the array) is the most significand.
  // For example, the number 1024 is hence represented by the digit sequence '1','0','2','4' of length 4.
  // Leading zeroes are not allowed.
  uint8_t* p;
  /// The length of the array pointed to by @a p.
  uint32_t n;
  /// The sign. Shizu_Boolean_True means plus and Shizu_Boolean_False means minus.
  /// If the value is zero, then the sign must be Shizu_Boolean_True.
  Shizu_Boolean sign;
};

struct Shizu_BigInteger_Dispatch {
  Shizu_Object_Dispatch _parent;
};

Shizu_Boolean
Shizu_BigInteger_isZero
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

Shizu_BigInteger*
Shizu_BigInteger_negate
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

Shizu_BigInteger*
Shizu_BigInteger_create
  (
    Shizu_State2* state
  );

Shizu_BigInteger*
Shizu_BigInteger_createFromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v
  );

#if 1 == Shizu_Configuration_WithInteger64

Shizu_BigInteger*
Shizu_BigInteger_createFromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v
  );

#endif

Shizu_BigInteger*
Shizu_BigInteger_createFromBigInteger
  (
    Shizu_State2* state,
    Shizu_BigInteger* v
  );

Shizu_Boolean
Shizu_BigInteger_isEqualTo
  (
    Shizu_State2* state,
    Shizu_BigInteger* self,
    Shizu_BigInteger* other
  );

Shizu_String*
Shizu_BigInteger_toString
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

void
Shizu_BigInteger_add
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

void
Shizu_BigInteger_multiply
  (
    Shizu_State2* state,
    Shizu_BigInteger* self
  );

#endif // SHIZU_RUNTIME_OBJECTS_BIGINTEGER_H_INCLUDED
