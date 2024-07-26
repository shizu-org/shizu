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

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// fprintf, stdout, stderr
#include <stdio.h>
// strlen
#include <string.h>

/* Test Shizu_powerOfTwoGreaterThan. */
static void
test1
  (
    Shizu_State2* state
  );

static void
test1
  (
    Shizu_State2* state
  )
{
  Shizu_State1* state1 = Shizu_State2_getState1(state);
  uint8_t result;
  result = Shizu_powerOfTwoGreaterThanOrEqualToU8(state1, 0);
  if (1 != result) {
    Shizu_State1_setStatus(state1, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state1);
  }
  result = Shizu_powerOfTwoGreaterThanOrEqualToU8(state1, 1);
  if (1 != result) {
    Shizu_State1_setStatus(state1, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state1);
  }
  result = Shizu_powerOfTwoGreaterThanOrEqualToU8(state1, 127);
  if (128 != result) {
    Shizu_State1_setStatus(state1, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state1);
  }
}

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
  if (safeExecute(&test1)) {
    failed = true;
  }
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
