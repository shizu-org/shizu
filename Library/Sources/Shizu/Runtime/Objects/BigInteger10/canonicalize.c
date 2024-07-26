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
#include "Shizu/Runtime/Objects/BigInteger10/canonicalize.h"

#include "Shizu/Runtime/State2.h"
#include <string.h>

void
_Shizu_BigInteger10_canonicalize
  (
    Shizu_State2* state,
    bool *sign,
    uint8_t **digits,
    uint32_t* numberOfDigits,
    bool reallocate
  )
{
  // Count all leading zeroes.
  uint32_t count = 0;
  for (uint32_t i = 0, n = (*numberOfDigits); i < n; ++i) {
    if ((*digits)[i]) {
      break;
     } else {
      count++;
     }
  }
  if (count) {
    // If count equals the number of digits, then the number only consists of leading zeroes.
    // Do not remove the last leading zero.
    if (count == (*numberOfDigits)) {
      count--;
    }
    memmove((*digits), (*digits) + count, (*numberOfDigits) - count);
    (*numberOfDigits) -= count;
    if (reallocate) {
      uint8_t* temporary = Shizu_State1_reallocate(Shizu_State2_getState1(state), (*digits), sizeof(uint8_t) * (*numberOfDigits));
      if (!temporary) {
        Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
        Shizu_State2_jump(state);
      }
      (*digits) = temporary;
    }
    // @todo Maybe add a boolean flag to shrink-realloc (*digits)?
  }
  // If the magnitude is 0 then we demand the sign to be positive.
  if ((*numberOfDigits) == 1 && (*digits)[0] == 0) {
    (*sign) = Shizu_Boolean_True;
  }
}
