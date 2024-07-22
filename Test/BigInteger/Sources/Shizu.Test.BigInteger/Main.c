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

#include "testArithmetic.h"
#include "testDefault.h"
#include "testFromInteger.h"
#include "testToInteger.h"
#include "testType.h"
#include "testFromString.h"

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
  // Default (constructor) (testDefault.h).
  if (safeExecute(&Shizu_Test_BigInteger_testDefault)) {
    failed = true;
  }
  // From integer (constructor) (testFromInteger.h).
  if (safeExecute(&Shizu_Test_BigInteger_testFromInteger32)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger_testFromInteger64)) {
    failed = true;
  }
  // From string (constructor) (testFromString.h).
  if (safeExecute(&Shizu_Test_BigInteger_testFromString)) {
    failed = true;
  }
  // Type (testType.h).
  if (safeExecute(&Shizu_Test_BigInteger_testType)) {
    failed = true;
  }
  // Arithmetic operations (testArithmetic.h).
  if (safeExecute(&Shizu_Test_BigInteger_testNegate)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger_testAddition)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger_testSubtraction)) {
    failed = true;
  }
  // To integer conversions (testToInteger.h).
  if (safeExecute(&Shizu_Test_BigInteger_testToInteger32)) {
    failed = true;
  }
  if (safeExecute(&Shizu_Test_BigInteger_testToInteger64)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
