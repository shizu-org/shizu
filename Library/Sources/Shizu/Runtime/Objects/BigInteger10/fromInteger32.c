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
#include "Shizu/Runtime/Objects/BigInteger10/fromInteger32.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/ByteArray.h"

void
_Shizu_BigInteger10_fromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v,
    Shizu_Boolean* sign,
    uint8_t** p,
    uint32_t* n
  )
{
  uint8_t* p1;
  uint32_t n1;
  Shizu_Boolean sign1;
  if (0 == v) {
    p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(uint8_t));
    if (!p1) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    p1[0] = 0;
    n1 = 1;
    sign1 = Shizu_Boolean_True;
    *p = p1;
    *n = n1;
    *sign = sign1;
    return;
  }
  Shizu_ByteArray* buffer = Shizu_ByteArray_create(state);
  if (Shizu_Integer32_Minimum == v) {
    // If v is Shizu_Integer32_Minimum, then -v would overflow.
    // Simply (v % 10) * -1 and v' = v / 10 to extract one digit and also make v > Shizu_Integer32_Minimum.
    uint8_t digit = (uint8_t)((v % 10) * -1);
    v /= 10;
    Shizu_ByteArray_appendRawBytes(state, buffer, &digit, sizeof(uint8_t));
  }
  if (v < 0) {
    sign1 = Shizu_Boolean_False;
    // We can safely do this now.
    v *= -1;
  } else {
    sign1 = Shizu_Boolean_True;
  }
  while (v != 0) {
    Shizu_Integer32 digit = (uint8_t)(v % 10);
    v /= 10;
    Shizu_ByteArray_appendRawBytes(state, buffer, &digit, sizeof(uint8_t));
  }
  uint8_t const* temporary = (uint8_t const*)Shizu_ByteArray_getRawBytes(state, buffer);
  n1 = Shizu_ByteArray_getNumberOfRawBytes(state, buffer);
  p1 = Shizu_State1_allocate(Shizu_State2_getState1(state), n1 * sizeof(uint8_t));
  if (!p1) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  // This also reverses the order.
  for (size_t i = 0; i < n1; ++i) {
    p1[n1 - 1 - i] = temporary[i];
  }
  *p = p1;
  *n = n1;
  *sign = sign1;
}
