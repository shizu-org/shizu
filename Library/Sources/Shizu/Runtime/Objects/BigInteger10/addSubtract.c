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
#include "Shizu/Runtime/Objects/BigInteger10/addSubtract.h"
#include "Shizu/Runtime/Objects/BigInteger10.h"
#include "Shizu/Runtime/Objects/BigInteger10/compareMagnitudes.h"
#include "Shizu/Runtime/Objects/BigInteger10/canonicalize.h"
#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Gc.h"

// The return value indicates if |x|-|y| or |y|-|x| was computed.
// If |x| >= |y| compute |x| - |y| and return true. Otherwise compute |y| - |x| and return false.
//  
static bool
subtractDigits
  (
    Shizu_State2* state,
    Shizu_BigInteger10* x,
    Shizu_BigInteger10* y,
    uint8_t** pz,
    Shizu_Integer32* pzn
  )
{
  Shizu_Integer32 compareResult = _Shizu_BigInteger10_compareMagnitudes(state, x, y);
  if (compareResult < 0) {
    Shizu_BigInteger10* t = x;
    x = y;
    y = t;
  }
  Shizu_Integer32 zn = x->n;
  uint8_t* z = Shizu_State1_allocate(Shizu_State2_getState1(state), zn);
  if (!z) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  bool borrow = false;
  uint8_t const* xp = x->p;
  uint8_t const* yp = y->p;
  Shizu_Integer32 xn = x->n;
  Shizu_Integer32 yn = y->n;
  // Subtract common part.
  while (yn > 0) {
    uint8_t u = xp[--xn];
    uint8_t v = yp[--yn] + (borrow ? 1 : 0);
    uint8_t r = (u < v) ? v - u : u - v;
    bool borrow = (u < v);
    z[xn] = r;
  }
  // Subtract part specific to the longer number while propagating borrow.
  while (xn > 0 && borrow) {
    uint8_t u = xp[--xn];
    uint8_t v = (borrow ? 1 : 0);
    uint8_t r = (u < v) ? v - u : u - v;
    bool borrow = (u < v);
    z[xn] = r;
  }
  // Subtract part specific to the longer number.
  while (xn > 0) {
    z[--xn] = xp[xn];
  }
  *pz = z;
  *pzn = zn;
  return compareResult >= 0;
}

static void
addDigits
  (
    Shizu_State2* state,
    Shizu_BigInteger10* x,
    Shizu_BigInteger10* y,
    uint8_t** pz,
    Shizu_Integer32* pzn
  )
{
  if (x->n < y->n) {
    Shizu_BigInteger10* t = x;
    x = y;
    y = t;
  }
  Shizu_Integer32 zn = x->n;
  uint8_t* z = Shizu_State1_allocate(Shizu_State2_getState1(state), zn);
  if (!z) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  bool carry = false;
  uint8_t const* xp = x->p;
  uint8_t const* yp = y->p;
  Shizu_Integer32 xn = x->n;
  Shizu_Integer32 yn = y->n;
  // Add common part.
  while (yn > 0) {
    uint8_t r = xp[--xn] + yp[--yn] + (carry ? 1 : 0);
    z[xn] = r % 10;
    carry = r / 10;
  }
  // Add part specific to the longer number while propagating carry.
  while (xn > 0 && carry) {
    carry = (0 == (z[--xn] = xp[xn] + 1));
  }
  // Add part specific to the longer number.
  while (xn > 0) {
    z[--xn] = xp[xn];
  }
  // Handle remaining carry (if any).
  if (carry) {
    uint8_t* w = Shizu_State1_allocate(Shizu_State2_getState1(state), zn + 1);
    if (!w) {
      Shizu_State1_deallocate(Shizu_State2_getState1(state), z);
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    memcpy(w + 1, z, zn);
    w[0] = carry;
    zn++;
    Shizu_State1_deallocate(Shizu_State2_getState1(state), z);
    z = w;
  }
  *pz = z;
  *pzn = zn;
}

Shizu_BigInteger10*
add
  (
    Shizu_State2* state,
    Shizu_BigInteger10* self,
    Shizu_BigInteger10* other
  )
{
  if (Shizu_BigInteger10_isZero(state, self)) {
    return other;
  }
  if (Shizu_BigInteger10_isZero(state, other)) {
    return self;
  }
  Shizu_Type* TYPE = Shizu_BigInteger10_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger10* RESULT = (Shizu_BigInteger10*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  // Neither "self" nor "other" is zero.
  // a) If their signs are equal, then this reduces to an addition of the magnitudes of "self" and "other" and the sign of the result is the sign of "self".
  // b) Otherwise we invoke "subtractDigits". If "subtractDigits" returns "true" then "self" >= "other" and hence the sign is the sign of "self".
  //    If "subtractDigits" returns "false" then "self" < "other" and the sign is the sign of "other".
  //    "subtractDigits" may return a number of leading zeroes, we also fix that.
  if (self->sign == other->sign) {
    // Signs are the same. Add magnitudes. Retain sign.
    RESULT->sign = self->sign;
    addDigits(state, self, other, &RESULT->p, &RESULT->n);
  } else {
    // Signs are different.
    bool result = subtractDigits(state, self, other, &RESULT->p, &RESULT->n);
    RESULT->sign = result ? self->sign : other->sign;
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      _Shizu_BigInteger10_canonicalize(state, &RESULT->sign, &RESULT->p, &RESULT->n, true);
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
      Shizu_State1_deallocate(Shizu_State2_getState1(state), RESULT->p);
      RESULT->p = NULL;
      Shizu_State2_jump(state);
    }
  }
  //
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}

Shizu_BigInteger10*
subtract
  (
    Shizu_State2* state,
    Shizu_BigInteger10* self,
    Shizu_BigInteger10* other
  )
{
  if (Shizu_BigInteger10_isZero(state, self)) {
    return Shizu_BigInteger10_negate(state, other);
  }
  if (Shizu_BigInteger10_isZero(state, other)) {
    return self;
  }
  Shizu_Type* TYPE = Shizu_BigInteger10_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_BigInteger10* RESULT = (Shizu_BigInteger10*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_Object_construct(state, (Shizu_Object*)RESULT);
  // Neither "self" nor "other" is zero. If their signs are different, then this reduces to an addition of the magnitudes of "self" and "other"
  // and the sign of the result is the sign of "self". Otherwise we invoke "subtractDigits". If "subtractDigits" returns "true" then "self"
  // >= "other" and hence the sign is the sign of "self". If "subtractDigits" returns "false" then "self" < "other" and the sign is the
  // sign of minus "self". "subtractDigits" may return a number of leading zeroes, we also fix that.
  if (self->sign != other->sign) {
    RESULT->sign = self->sign;
    addDigits(state, self, other, &RESULT->p, &RESULT->n);
  } else {
    bool result = subtractDigits(state, self, other, &RESULT->p, &RESULT->n);
    RESULT->sign = result ? self->sign : !self->sign;
    // Remove leading zeroes and adjust sign.
    size_t count = 0;
    for (size_t i = 0, n = RESULT->n; i < n; ++i) {
      if (RESULT->p[0]) {
        break;
      } else {
        count++;
      }
    }
    if (count) {
      // Do not remove the only remaining zero.
      if (count == RESULT->n) {
        count--;
      }
      memmove(RESULT->p, RESULT->p + count, RESULT->n - count);
      RESULT->n -= count;
    }
    if (RESULT->n == 1 && !RESULT->p[0]) {
      RESULT->sign = Shizu_Boolean_True;;
    }
  }
  //
  ((Shizu_Object*)RESULT)->type = TYPE;
  return RESULT;
}
