#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/StringToInteger/Version1/Include.h"

#include "Shizu/Runtime/Operations/StringToInteger/Version1/Parser.h"
#include "Shizu/Runtime/Operations/Utilities/BigInt/Include.h"

typedef struct ConversionState {
  bigint_t* digits;
} ConversionState;

static void
Conversion_destruct
  (
    Shizu_State1* state,
    ConversionState* conversionState
  )
{
  if (conversionState->digits) {
    bigint_free(state, conversionState->digits);
    conversionState->digits = NULL;
  }
}

// Basically the callback function creates to big integers in context.
static void
callbackFunction
  (
    Shizu_State2* state,
    void* context,
    bool sign,
    char const* digits,
    size_t digitsCount
  )
{
  ConversionState* c = (ConversionState*)context;

  // Count the number of leading zeroes.
  // If the number consists only of zeroes, do not count one zero.
  size_t numberOfLeadingZeroes = 0;
  for (size_t i = 0, n = digitsCount; i < n; ++i) {
    if ('0' != digits[i]) {
      break;
    }
    numberOfLeadingZeroes++;
  }
  if (numberOfLeadingZeroes == digitsCount) {
    numberOfLeadingZeroes--;
  }
  // Determine the size and the capacity.
  size_t sz = digitsCount - numberOfLeadingZeroes;
  size_t cp = sz;
  //
  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  x->cp = cp;
  x->sz = sz;
  x->p = malloc(sizeof(uint8_t) * cp);
  if (!x->p) {
    free(x);
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  };
  // Read the digits in their textual order (MBD to LSD).
  for (size_t i = 0, j = numberOfLeadingZeroes, n = digitsCount; i < n;) {
    x->p[j++] = (uint8_t)(digits[i++] - '0');
  }
  // Reverse MSD to LSD to LSD to MSD.
  for (size_t i = 0, n = sz / 2; i < n; ++i) {
    uint8_t digit = x->p[i];
    x->p[i] = x->p[sz - i - 1];
    x->p[sz - i - 1] = digit;
  }
  // Determine sign.
  if (x->sz == 1 && x->p[0] == 0) {
    x->sign = 0;
  } else {
    x->sign = sign ? +1 : -1;
  }
  c->digits = x;
}

Shizu_Integer32
Shizu_Operations_StringToInteger32_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  )
{
  Shizu_State1* state1 = Shizu_State2_getState1(state);
  ConversionState context = { .digits = NULL, };
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Operations_StringToInteger_Parser_parse(state, source, &context, &callbackFunction);
    Shizu_Cxx_Debug_assert(NULL != context.digits);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Conversion_destruct(state1, &context);
    Shizu_State2_jump(state);
  }
  // If we could store somewhere Shizu_Integer32_Minimum and Shizu_Integer32_Maximum as bigint_t values,
  // then a test if this would overflow would be very fast.
  if (bigint_compare_i32(state1, context.digits, Shizu_Integer32_Minimum) < 0) {
    Conversion_destruct(state1, &context);
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  if (bigint_compare_i32(state1, context.digits, Shizu_Integer32_Maximum) > 0) {
    Conversion_destruct(state1, &context);
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Integer32 v = 0;
  for (size_t i = context.digits->sz; i > 0; --i) {
    uint8_t digit = context.digits->p[i - 1];
    v *= 10;
    v += digit;
  }
  v *= context.digits->sign;
  Conversion_destruct(state1, &context);
  return v;
}

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer64
Shizu_Operations_StringToInteger64_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  )
{
  Shizu_State1* state1 = Shizu_State2_getState1(state);
  ConversionState context = { .digits = NULL, };
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Operations_StringToInteger_Parser_parse(state, source, &context, &callbackFunction);
    Shizu_Cxx_Debug_assert(NULL != context.digits);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Conversion_destruct(state1, &context);
    Shizu_State2_jump(state);
  }
  // If we could store somewhere Shizu_Integer64_Minimum and Shizu_Integer64_Maximum as bigint_t values,
  // then a test if this would overflow would be very fast.
  if (bigint_compare_i64(state1, context.digits, Shizu_Integer64_Minimum) < 0) {
    Conversion_destruct(state1, &context);
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  if (bigint_compare_i64(state1, context.digits, Shizu_Integer64_Maximum) > 0) {
    Conversion_destruct(state1, &context);
    Shizu_State2_setStatus(state, Shizu_Status_ConversionFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Integer64 v = 0;
  for (size_t i = context.digits->sz; i > 0; --i) {
    uint8_t digit = context.digits->p[i - 1];
    v *= 10;
    v += digit;
  }
  v *= context.digits->sign;
  Conversion_destruct(state1, &context);
  return v;
}

#endif
