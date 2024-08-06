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

/* Test Shizu_Operations_toInteger32 with a string as its input. */
static void
test1_i32
  (
    Shizu_State2* state
  );


/* Test Shizu_Operations_toInteger64 with a string as its input. */
static void
test1_i64
  (
    Shizu_State2* state
  );

static void
test1_i32
  (
    Shizu_State2* state
  )
{
#define TEST(receivedInput, expectedOutput) \
  {  \
    Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void); \
    Shizu_Value argumentValues[] = { \
      Shizu_Value_InitializerObject(Shizu_String_create(state, receivedInput, strlen(receivedInput))) \
    }; \
    Shizu_Operations_toInteger32(state, &returnValue, 1, &argumentValues[0]); \
    if (!Shizu_Value_isInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError); \
      Shizu_State2_jump(state); \
    } \
    if (expectedOutput != Shizu_Value_getInteger32(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError); \
      Shizu_State2_jump(state); \
    } \
  }

  TEST("0", 0);
  TEST("-2147483648", Shizu_Integer32_Minimum);
  TEST("+2147483647", Shizu_Integer32_Maximum);

#undef TEST
}

#if 1 == Shizu_Configuration_WithInteger64

static void
test1_i64
  (
    Shizu_State2* state
  )
{
#define TEST(receivedInput, expectedOutput) \
  {  \
    Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void); \
    Shizu_Value argumentValues[] = { \
      Shizu_Value_InitializerObject(Shizu_String_create(state, receivedInput, strlen(receivedInput))) \
    }; \
    Shizu_Operations_toInteger64(state, &returnValue, 1, &argumentValues[0]); \
    if (!Shizu_Value_isInteger64(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError); \
      Shizu_State2_jump(state); \
    } \
    if (expectedOutput != Shizu_Value_getInteger64(&returnValue)) { \
      Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError); \
      Shizu_State2_jump(state); \
    } \
  }

  TEST("0", 0);
  TEST("-9223372036854775808", Shizu_Integer64_Minimum);
  TEST("+9223372036854775807", Shizu_Integer64_Maximum);

#undef TEST
}

#endif

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
  if (safeExecute(&test1_i32)) {
    failed = true;
  }
#if 1 == Shizu_Configuration_WithInteger64
  if (safeExecute(&test1_i64)) {
    failed = true;
  }
#endif
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
