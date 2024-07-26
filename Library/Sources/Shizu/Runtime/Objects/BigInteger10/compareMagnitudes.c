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
#include "Shizu/Runtime/Objects/BigInteger10/compareMagnitudes.h"
#include "Shizu/Runtime/Objects/BigInteger10.h"

Shizu_Integer32
_Shizu_BigInteger10_compareMagnitudes
  (
    Shizu_State2* state,
    Shizu_BigInteger10* x,
    Shizu_BigInteger10* y
  )
{
  if (x->n < y->n) {
    // This is possible as there are no leading zeroes.
    return -1;
  } else if (x->n > y->n) {
    // This is possible as there are no leading zeroes.
    return +1;
  } /*else if (xn == yn)*/ {
    return memcmp(x->p, y->p, x->n);
  }
}
