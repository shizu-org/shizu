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

#include "Shizu/Runtime/Include.h"

#include "Shizu.Test.BigInteger10/testArithmetic.h"
#include "Shizu.Test.BigInteger10/testDefault.h"
#include "Shizu.Test.BigInteger10/testFromInteger.h"
#include "Shizu.Test.BigInteger10/testToInteger.h"
#include "Shizu.Test.BigInteger10/testType.h"
#include "Shizu.Test.BigInteger10/testFromString.h"

#include "Shizu.Test.BigInteger2/testType.h"

static int
safeExecute
  (
    void (*test)(Shizu_State2* state)
  )
{
  if (!test) {
    return 1;
  }
  Shizu_State2* state = NULL;
  if (Shizu_State2_acquire(&state)) {
    return 1;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State2_ensureModulesLoaded(state);
    (*test)(state);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_relinquish(state);
    state = NULL;
    return 1;
  }
  Shizu_State2_relinquish(state);
  state = NULL;
  return 0;
}

int
main
  (
    int argc,
    char** argv
  )
{
  bool failed = false;
  // Type (testType.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testType)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger2_testType)) {
    failed = true;
  }
  // Default (constructor) (testDefault.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testDefault)) {
    failed = true;
  }
  // From integer (constructor) (testFromInteger.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testFromInteger32)) {
    failed = true;
  }
#if 1 == Shizu_Configuration_WithInteger64
  if (safeExecute(&Shizu_Test_BigInteger10_testFromInteger64)) {
    failed = true;
  }
#endif
  // From string (constructor) (testFromString.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testFromString)) {
    failed = true;
  }
  // Arithmetic operations (testArithmetic.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testNegate)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testAddition)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testSubtraction)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testMultiplyBy10)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testDivideBy10)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testMultiply)) {
    failed = true;
  }
  // To integer conversions (testToInteger.h).
  if (safeExecute(&Shizu_Test_BigInteger10_testToInteger32)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger10_testToInteger64)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
