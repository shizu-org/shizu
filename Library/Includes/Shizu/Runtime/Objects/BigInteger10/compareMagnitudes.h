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

#if !defined(SHIZU_OBJECTS_BIGINTEGER10_COMPAREMAGNITUDES_H_INCLUDED)
#define SHIZU_OBJECTS_BIGINTEGER10_COMPAREMAGNITUDES_H_INCLUDED

#include "Shizu/Runtime/Value.h"
typedef struct Shizu_BigInteger10 Shizu_BigInteger10;

/// @param x The big integer.
/// @param y The big integer.
/// @return
/// <code>-1</code> (<code>+1</code>) if the magnitude of <code>x</code> is smaller (greater) than the magnitude <code>y</code>.
/// <code>0</code> otherwise.
Shizu_Integer32
_Shizu_BigInteger10_compareMagnitudes
  (
    Shizu_State2* state,
    Shizu_BigInteger10* x,
    Shizu_BigInteger10* y
  );

#endif // SHIZU_OBJECTS_BIGINTEGER10_COMPAREMAGNITUDES_H_INCLUDED

