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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Objects/BigInteger2/fromBigInteger10.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/BigInteger10.h"

// The longest sequence of decimal digits that is representable by an uint32_t REGARDLESS of the digits value.
//
// The UINT32_MAX decimal value is 4294967295 and is of length 10. However, the sequence 9999999999 of length 10 exceeds that value.
// In fact, the longest sequence of decimal digits that is representable by an uint32_t REGARDLESS of the digit values is of length 9 (which is the sequence 999999999).
//
// This is used when converting a sequence of base 10 digits.
#define DIGITS_PER_UINT32_BASE10 UINT32_C(9)

Shizu_Cxx_staticAssert(DIGITS_PER_UINT32_BASE10 < UINT32_MAX, "<internal error>");

// This number is effectively 10^9 = 1000000000.
#define SHIFT_BASE10 UINT32_C(1000000000)

Shizu_Cxx_staticAssert(1000000000 < UINT32_MAX, "<internal error>");

static void
mulAdd
  (
    uint32_t* x,
    uint32_t xl,
    uint32_t y,
    uint32_t z
  )
{
  // perform multiplication
  uint64_t product = 0;
  uint64_t carry = 0;
  for (uint32_t i = xl; i > 0; --i) {
    product = ((uint64_t)y) * ((uint64_t)x[i - 1]) + carry;
    x[i] = (uint32_t)product;
    carry = product >> 32;
  }

  // perform addition
  uint64_t sum = ((uint64_t)x[xl - 1]) + (uint64_t)z;
  x[xl - 1] = (uint32_t)sum;
  carry = sum >> 32;
  for (uint32_t i = xl - 1; i > 0; --i) {
    sum = ((uint64_t)x[i - 1]) + carry;
    x[i] = (uint32_t)sum;
    carry = sum >> 32;
  }
}

static uint32_t
transcodeDigits
  (
    uint8_t const** start,
    uint8_t const* end
  )
{
  uint32_t w = 0;
  uint32_t count = 0;
  while ((*start) < end && count < DIGITS_PER_UINT32_BASE10) {
    w *= 10;
    w += (**start);
    (*start)++;
    count++;
  }
  return w;
}

void
_Shizu_BigInteger2_fromBigInteger10
  (
    Shizu_State2* state,
    Shizu_BigInteger10* source,
    Shizu_Boolean* sign,
    uint32_t** p,
    uint32_t* n
  )
{
  if (source->n == 1 && source->p[0] == 0) {
    uint32_t* q = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint32_t));
    if (!q) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    q[0] = UINT32_C(0);
    *p = q;
    *n = 1;
    *sign = Shizu_Boolean_True;
  } else {
    // The following reasoning provides a good and safe estimate of the required number of uint32_t values to represent the decimal number.
    // We have n decimal digits. A decimal digit has a maximal value of 9. The binary representation of 9 is 1001.
    // Hence we require four bits per decimal digit.
    // Hence the number of bits required to store all n decimal digits is 4 * n.
    // Hence the number of uint32_t values required to store all n decimal digits is ceil(4 * n / 32).
    // A save approximation is hence 4 * n / 32 + 1 or n / 8 + 1.  
    uint32_t numberOfWords = (source->n / 8) + 1;
    uint32_t* words = Shizu_State1_allocate(Shizu_State2_getState1(state), numberOfWords * sizeof(uint32_t));
    if (!words) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    for (uint32_t i = 0; i < numberOfWords; ++i) {
      words[i] = 0;
    }
    uint8_t const* start = source->p;
    uint8_t const* end = source->p + source->n;
    uint32_t v = transcodeDigits(&start, end);
    words[numberOfWords - 1] = v;
    while (start != end) {
      v = transcodeDigits(&start, end);
      mulAdd(words, numberOfWords, SHIFT_BASE10, v);
    }
    // Remove leading zeroes and adjust sign.
    uint32_t numberOfLeadingZeroWords = 0;
    for (uint32_t i = 0, n = numberOfWords; i < n; ++i) {
      if (words[0]) {
        break;
      } else {
        numberOfLeadingZeroWords++;
      }
    }
    if (numberOfLeadingZeroWords > 0) {
      numberOfWords -= numberOfLeadingZeroWords;
      memmove(words, words + numberOfLeadingZeroWords, numberOfWords);
    }
    *p = words;
    *n = numberOfWords;
    *sign = source->sign;
  }
}
