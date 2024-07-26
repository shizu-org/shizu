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
#include "Shizu/Runtime/Objects/BigInteger2/fromInteger32.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/ByteArray.h"

void
_Shizu_BigInteger2_fromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v,
    Shizu_Boolean* sign,
    uint32_t** p,
    uint32_t* n
  )
{
  uint32_t* p1;
  uint32_t n1;
  Shizu_Boolean sign1;
  if (Shizu_Integer32_Minimum == v) {
    // If v is Shizu_Integer32_Minimum, then -v would overflow.
    // Simply hard-code the corresponding magnitude.
    p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), 1 * sizeof(uint32_t));
    if (!p1) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    p1[0] = 0x80000000;
    n1 = 1;
    sign1 = Shizu_Boolean_False;
    *p = p1;
    *n = n1;
    *sign = sign1;
    return;
  }
  Shizu_Cxx_Debug_assert(v > Shizu_Integer32_Minimum);
  if (v < 0) {
    sign1 = Shizu_Boolean_False;
    v = -v;
  }
  Shizu_Cxx_Debug_assert(v > Shizu_Integer32_Minimum && v < 0);
  p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint32_t));
  if (!p1) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  p1[0] = v;
  n1 = 1;
  *p = p1;
  *n = n1;
  *sign = sign1;
}
