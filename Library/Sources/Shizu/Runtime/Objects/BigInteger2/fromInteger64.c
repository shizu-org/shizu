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
#include "Shizu/Runtime/Objects/BigInteger10/fromInteger64.h"

#if 1 == Shizu_Configuration_WithInteger64

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/ByteArray.h"

void
_Shizu_BigInteger2_fromInteger64
  (
    Shizu_State2* state,
    Shizu_Integer64 v,
    Shizu_Boolean* sign,
    uint32_t** p,
    uint32_t* n
  )
{
  uint32_t* p1;
  uint32_t n1;
  Shizu_Boolean sign1;
  if (Shizu_Integer64_Minimum == v) {
    // If v is Shizu_Integer64_Minimum, then -v would overflow.
    // Simply hard-code the corresponding magnitude.
    p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), 2 * sizeof(uint32_t));
    if (!p1) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    p1[0] = UINT32_C(0x80000000);
    p1[1] = UINT32_C(0x00000000);
    n1 = 2;
    sign1 = Shizu_Boolean_False;
    *p = p1;
    *n = n1;
    *sign = sign1;
    return;
  }
  Shizu_Cxx_Debug_assert(v > Shizu_Integer64_Minimum);
  if (v < 0) {
    sign1 = Shizu_Boolean_False;
    v = -v;
  }
  Shizu_Cxx_Debug_assert(v > Shizu_Integer64_Minimum && v < 0);
  int32_t hi = (int32_t)(v >> 32);
  int32_t lo = (int32_t)(v >> 0);
  if (hi == 0) {
    n1 = 1; 
  } else {
    n1 = 2;
  }
  p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), n1 * sizeof(uint32_t));
  if (!p1) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  uint64_t w = (uint64_t)v;
  if (hi == 0) {
    p1[0] = lo;
  } else {
    p1[0] = hi;
    p1[1] = lo;
  }
  *p = p1;
  *n = n1;
  *sign = sign1;
}

#endif // 1 == Shizu_Configuration_WithInteger64

