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

#if !defined(SHIZU_TEST_BIGINTEGER10_TESTARITHMETIC_H_INCLUDED)
#define SHIZU_TEST_BIGINTEGER10_TESTARITHMETIC_H_INCLUDED

#include "Shizu/Runtime/Include.h"
// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// fprintf, stdout, stderr
#include <stdio.h>
// strlen
#include <string.h>

void
Shizu_Test_BigInteger10_testNegate
  (
    Shizu_State2* state
  );

void
Shizu_Test_BigInteger10_testAddition
  (
    Shizu_State2* state
  );

void
Shizu_Test_BigInteger10_testSubtraction
  (
    Shizu_State2* state
  );

void
Shizu_Test_BigInteger10_testMultiplyBy10
  (
    Shizu_State2* state
  );

void
Shizu_Test_BigInteger10_testDivideBy10
  (
    Shizu_State2* state
  );

void
Shizu_Test_BigInteger10_testMultiply
  (
    Shizu_State2* state
  );

#endif // SHIZU_TEST_BIGINTEGER10_TESTARITHMETIC_H_INCLUDED
