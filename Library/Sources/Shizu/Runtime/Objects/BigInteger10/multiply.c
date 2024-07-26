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
#include "Shizu/Runtime/Objects/BigInteger10/multiply.h"

#include "Shizu/Runtime/Objects/BigInteger10/canonicalize.h"
#include "Shizu/Runtime/Objects/BigInteger10.h"
#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Gc.h"

void
_Shizu_BigInteger10_multiply
  ( 
    Shizu_State2* state,
    Shizu_BigInteger10* self,
    Shizu_BigInteger10* other,
    bool* pSign,
    uint8_t** pDigits,
    Shizu_Integer32* pNumberOfDigits
  )
{
  Shizu_BigInteger10* x = self;
  Shizu_BigInteger10* y = other;
  
  // The maximum length of a product of x and y is len(x) + len(y).
  uint32_t numberOfDigits = x->n + y->n;
  uint8_t* digits = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint8_t) * numberOfDigits);
  if (!digits) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  for (size_t i = 0; i < numberOfDigits; ++i) {
    digits[i] = 0;
  }

  size_t xstart = x->n;
  size_t ystart = y->n;

  uint16_t carry = 0;
  for (size_t j = ystart, k = ystart + xstart; j > 0; --j, --k) {
    uint16_t product = (uint16_t)y->p[j - 1]
                     * (uint16_t)x->p[xstart - 1]
                     + carry;
    digits[k - 1] = (uint8_t)(product % 10);
    carry = product / 10;
  } 
  digits[xstart - 1] = (uint8_t)carry;
  
  for (size_t i = xstart - 1; i > 0; --i) {
    carry = 0;
    for (size_t j = ystart, k = ystart + i; j > 0; --j, --k) {
      uint16_t product = (uint16_t)(y->p[j - 1])
                       * (uint16_t)(x->p[i - 1])
                       + (uint16_t)(digits[k - 1])
                       + carry;
      digits[k - 1] = (uint8_t)product;
      carry = product / 10;
    }
    digits[i - 1] = (uint8_t)carry;
  }
  bool sign = x->sign == y->sign;
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    _Shizu_BigInteger10_canonicalize(state, &sign, &digits, &numberOfDigits, true);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_deallocate(Shizu_State2_getState1(state), digits);
    digits = NULL;
    Shizu_State2_jump(state);
  }
  (*pSign) = sign;
  (*pDigits) = digits;
  (*pNumberOfDigits) = numberOfDigits;
}
