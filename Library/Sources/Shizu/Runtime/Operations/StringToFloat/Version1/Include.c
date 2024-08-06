#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/StringToFloat/Version1/Include.h"

#include "Shizu/Runtime/floatBits.h"
#include "Shizu/Runtime/Operations/StringToFloat/Version1/Parser.h"
#include "Shizu/Runtime/Operations/Utilities/BigInt/Include.h"
#include "Shizu/Runtime/countLeadingZeroes.h"
#include "Shizu/Runtime/countTrailingZeroes.h"

// Map from an exponent value i = 0, 1, 2, ..., 10 to the corresponding 10^i.
static const float FLT_SMALL_10_POW[] = {
  1.0e0f,
  1.0e1f,
  1.0e2f,
  1.0e3f,
  1.0e4f,
  1.0e5f,
  1.0e6f,
  1.0e7f,
  1.0e8f,
  1.0e9f,
  1.0e10f
};

static const size_t FLT_MAX_SMALL_TEN = (sizeof(FLT_SMALL_10_POW) / sizeof(float)) - 1;

// Map from an exponent value i = 0, 1, 2, ..., 22 to the corresponding 10^i.
static const double DBL_SMALL_10_POW[] = {
  1.0e0,
  1.0e1,
  1.0e2,
  1.0e3,
  1.0e4,
  1.0e5,
  1.0e6,
  1.0e7,
  1.0e8,
  1.0e9,
  1.0e10,
  1.0e11,
  1.0e12,
  1.0e13,
  1.0e14,
  1.0e15,
  1.0e16,
  1.0e17,
  1.0e18,
  1.0e19,
  1.0e20,
  1.0e21,
  1.0e22,
};

static const size_t DBL_MAX_SMALL_TEN = (sizeof(DBL_SMALL_10_POW) / sizeof(double)) - 1;

static const double DBL_TINY_10_POW[] = {
  1e-16,
  1e-32,
  1e-64,
  1e-128,
  1e-256
};

static const size_t DBL_MAX_TINY_10_POW = (sizeof(DBL_TINY_10_POW) / sizeof(double)) - 1;

typedef struct ConversionState {
  bigint_t* significand;
  bigint_t* exponent;
} ConversionState;

static void
Conversion_destruct
  (
    Shizu_State1* state,
    ConversionState* conversionState
  )
{
  if (conversionState->significand) {
    bigint_free(state, conversionState->significand);
    conversionState->significand = NULL;
  }
  if (conversionState->exponent) {
    bigint_free(state, conversionState->exponent);
    conversionState->exponent = NULL;
  }
}

static void
makeExp
  (
    Shizu_State2* state,
    ConversionState* c,
    bool exponentSign,
    char const* exponentDigits,
    size_t exponentDigitsCount
  )
{  
  // Count the number of leading zeroes to trim.
  size_t numberOfLeadingZeroes = 0;
  for (size_t i = 0, n = exponentDigitsCount; i < n; ++i) {
    if ('0' != exponentDigits[i]) {
      break;
    }
    numberOfLeadingZeroes++;
  }
  size_t cp = (exponentDigitsCount - numberOfLeadingZeroes);
  if (!cp) {
    cp = 1;
  }
  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  x->p = malloc(sizeof(uint8_t) * cp);
  x->cp = cp;
  if (!x->p) {
    free(x);
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  size_t j = 0;
  // Read the digits in their textual order (MSD to LSD).
  for (size_t i = numberOfLeadingZeroes, n = exponentDigitsCount; i < n; ++i) {
    x->p[j] = (uint8_t)(exponentDigits[i] - '0');
  }
  // Reverse MSD to LSD to LSD to MSD.
  for (size_t i = 0, n = j / 2; i < n; ++i) {
    uint8_t digit = x->p[i];
    x->p[i] = x->p[j - i - 1];
    x->p[j - i - 1] = digit;
  }
  x->sz = j;
  if (!x->sz) {
    // No digits copied at all?
    x->sz = 1;
    x->p[0] = 0;
  }
  if (x->sz == 1 && x->p[0] == 0) {
    x->sign = 0;
  } else {
    x->sign = exponentSign ? +1 : -1;
  }
  c->exponent = x;
}

// Basically the callback function creates two big integers in context.
// such that the original floating pointer number v.w * 10^e is expressed
// as .vw * 10^(e + |v|).
static void
callbackFunction
  (
    Shizu_State2* state,
    void* context,
    bool sign,
    char const* significandDigits,
    size_t significandDigitsCount,
    char const* fractionalDigits,
    size_t fractionalDigitsCount,
    bool exponentSign,
    char const* exponentDigits,
    size_t exponentDigitsCount
  )
{
  ConversionState* c = (ConversionState*)context;

  makeExp(state, c, exponentSign, exponentDigits, exponentDigitsCount);

  // Count the number of leading zeroes to trim before the decimal point.
  size_t numberOfLeadingZeroes = 0;
  for (size_t i = 0, n = significandDigitsCount; i < n; ++i) {
    if ('0' != significandDigits[i]) {
      break;
    }
    numberOfLeadingZeroes++;
  }
  // Modify the input "v.w * 10^e" to ".vw * 10^(e + |v|)".
  size_t shift = significandDigitsCount - numberOfLeadingZeroes;
  if (shift) {
    static_assert(SIZE_MAX <= UINT64_MAX, "<internal error>");
    bigint_in_situ_add_u64(Shizu_State2_getState1(state), c->exponent, shift);
  }

  // Count the number of trailing zeroes to trim after the decimal point.
  size_t numberOfTrailingZeroes = 0;
  for (size_t i = fractionalDigitsCount; i > 0; --i) {
    if ('0' != fractionalDigits[i - 1]) {
      break;
    }
    numberOfTrailingZeroes++;
  }
  size_t cp = (significandDigitsCount - numberOfLeadingZeroes) + (fractionalDigitsCount - numberOfTrailingZeroes);
  if (!cp) {
    cp = 1;
  }
  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  x->p = malloc(sizeof(uint8_t) * cp);
  x->cp = cp;
  if (!x->p) {
    free(x);
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  size_t j = 0;
  // Read the digits in their textual order (MBD to LSD).
  for (size_t i = numberOfLeadingZeroes, n = significandDigitsCount; i < n; ++i) {
    x->p[j++] = (uint8_t)(significandDigits[i] - '0');
  }
  for (size_t i = 0, n = fractionalDigitsCount - numberOfTrailingZeroes; i < n; ++i) {
    x->p[j++] = (uint8_t)(fractionalDigits[i] - '0');
  }
  // Reverse MSD to LSD to LSD to MSD.
  for (size_t i = 0, n = j / 2; i < n; ++i) {
    uint8_t digit = x->p[i];
    x->p[i] = x->p[j - i - 1];
    x->p[j - i - 1] = digit;
  }
  x->sz = j;
  if (!x->sz) {
    // No digits copied at all?
    x->sz = 1;
    x->p[0] = 0;
  }
  if (x->sz == 1 && x->p[0] == 0) {
    x->sign = 0;
  } else {
    x->sign = sign ? +1 : -1;
  }
  c->significand = x;
}

double clamp_f64(Shizu_State2* state, double v, double minimum, double maximum) {
  if (minimum > maximum) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  if (v < minimum) {
    v = minimum;
  } else if (v > maximum) {
    v = maximum;
  }
  return v;
}

float clamp_f32(Shizu_State2* state, float v, float minimum, float maximum) {
  if (minimum > maximum) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
    Shizu_State2_jump(state);
  }
  if (v < minimum) {
    v = minimum;
  } else if (v > maximum) {
    v = maximum;
  }
  return v;
}

int32_t min_s32(Shizu_State1* state, int32_t x, int32_t y) {
  return x < y ? x : y;
}

int32_t max_s32(Shizu_State1* state, int32_t x, int32_t y) {
  return x > y ? x : y;
}

size_t min_sz(Shizu_State1* state, size_t x, size_t y) {
  return x < y ? x : y;
}

// The double type has 52+1 significand bits.
// It hence can store approximatly floor(log10(2^53)) = floor(15.9545897702) = 15 decimal digits.
#define DBL_SIG_MAX_DECIMAL_DIGITS (15)

// The float type has 23+1 significand bits.
// It hence can represent approximatly floor(log10(2^24)) = floor(7.22471989594) = 7 decimal digits.
#define FLT_SIG_MAX_DECIMAL_DIGITS (7)

Shizu_Float64
Shizu_Operations_StringToFloat64_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  )
{
  Shizu_State1* state1 = Shizu_State2_getState1(state);
  ConversionState context = { .exponent = NULL, .significand = NULL };
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Operations_StringToFloat_Parser_parse(state, source, &context, &callbackFunction);
    // [1] At this point, we have two sign-magnitude arbitrary precision integers x and y
    // such that d = x * 10^y where d is the number represented by the input string.
    Shizu_Cxx_Debug_assert(NULL != context.exponent);
    Shizu_Cxx_Debug_assert(NULL != context.significand);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Conversion_destruct(state1, &context);
    Shizu_State1_jump(state1);
  }
  if (bigint_is_zero(state1, context.significand)) {
    // [2] If x is zero, then the result is zero.
    Conversion_destruct(Shizu_State2_getState1(state), &context);
    return 0.;
  }
  // [3] When mapping numbers from decimal to binary, we map from a * 10^q to b * 2^p.
  // What we attempt first summing up the magnitude in a long.
  // We consume up to min(totalDigitsCount, DBL_SIG_MAX_DECIMAL_DIGITS + 1).

  size_t totalDigitsCount = context.significand->sz;
  size_t prefixDigitsCount = min_sz(Shizu_State2_getState1(state), totalDigitsCount, DBL_SIG_MAX_DECIMAL_DIGITS + 1);
  uint64_t v_ui = 0;
  // Example: 7 digits and 5 prefix then i = 7 and n = 2 hence indices are [7,6,5,4,3].
  for (size_t i = totalDigitsCount, n = totalDigitsCount - prefixDigitsCount; i > n; --i) {
    v_ui = v_ui * 10 + context.significand->p[i - 1];
  }
  double v_d = (double)v_ui;

  if (context.exponent->sz > INT64_DECIMAL_DIG - 1) {
    // The maximum number of decimal digits required to represent any int64_t value.
    // Subtracting one yields the maximum number of decimal digits that can be accumulated in an int64_t regardless of the values of the digits.
    return Shizu_Float64_Infinity;
  }
  int64_t exp = 0;
  for (size_t i = context.exponent->sz; i > 0; --i) {
    exp = exp * 10 + context.exponent->p[i - 1];
  }
  // Our bigints basically are of the format .w * 10^p.
  // Now, we multiply .w by 10^prefixDigitsCount.
  // We make up for that by reducing the exponent by prefixDigitsCount.
  int32_t e = exp - prefixDigitsCount;
  // v_ui contains the accumulation of the first prefixDigitCount digits of the number.
  // v_d contains the corresponding double.
  if (prefixDigitsCount <= DBL_SIG_MAX_DECIMAL_DIGITS) {
    if (bigint_is_zero(Shizu_State2_getState1(state), context.exponent) || v_d == 0.0) {
      v_d = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_d : +v_d; // small floating integer
      Conversion_destruct(Shizu_State2_getState1(state), &context);
      return v_d;
    }

    if (e >= 0) {
      if (e <= DBL_MAX_SMALL_TEN) {
        v_d *= DBL_SMALL_10_POW[e];
        v_d = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_d : +v_d;
        Conversion_destruct(Shizu_State2_getState1(state), &context);
        return v_d;
      }
      size_t slop = DBL_SIG_MAX_DECIMAL_DIGITS - prefixDigitsCount;
      if (e <= DBL_MAX_SMALL_TEN + slop) {
        // We can multiply v_d by 10^(slop) and it is still "small" and exact.
        // Then we can multiply by 10^(exp-slop) with one rounding.
        v_d *= DBL_SMALL_10_POW[slop];
        v_d *= DBL_SMALL_10_POW[e - slop];
        v_d = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_d : +v_d;
        Conversion_destruct(Shizu_State2_getState1(state), &context);
        return v_d;
      }
      // Hard case with positive exponent.
    } else {
      if (e >= -DBL_MAX_SMALL_TEN) {
        // Can get the answer in one division.
        v_d /= DBL_SMALL_10_POW[-e];
        v_d = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_d : v_d;
        Conversion_destruct(Shizu_State2_getState1(state), &context);
        return v_d;
      }
      // Hard case with negative exponent.
    }
  } 
  // Harder cases:
  // The sum of digits plus exponent is greater than what we think we can do with one error.
  // Start by approximating the right answer by, naively, scaling by powers of 10.
  if (e > 0) {
    if (exp > DBL_MAX_10_EXP + 1) {
      // This is going to be infinity.
      return bigint_is_negative(state1, context.significand) ? -Shizu_Float64_Infinity : +Shizu_Float64_Infinity;
    }
  }
  Conversion_destruct(state1, &context);
  return 0.;
}

Shizu_Float32
Shizu_Operations_StringToFloat32_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  )
{
  Shizu_State1* state1 = Shizu_State2_getState1(state);
  ConversionState context = { .exponent = NULL, .significand = NULL };
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Operations_StringToFloat_Parser_parse(state, source, &context, &callbackFunction);
    // [1] At this point, we have two sign-magnitude arbitrary precision integers x and y
    // such that d = x * 10^y where d is the number represented by the input string.
    Shizu_Cxx_Debug_assert(NULL != context.exponent);
    Shizu_Cxx_Debug_assert(NULL != context.significand);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Conversion_destruct(state1, &context);
    Shizu_State2_jump(state);
  }
  if (bigint_is_zero(state1, context.significand)) {
    // [2] If x is zero, then the result is zero.
    Conversion_destruct(state1, &context);
    return 0.f;
  }
  // [3] When mapping numbers from decimal to binary, we map from a * 10^q to b * 2^p.
  // What we attempt first summing up the magnitude in a long.
  // We consume up to min(totalDigitsCount, FLT_SIG_MAX_DECIMAL_DIGITS + 1).
  size_t totalDigitsCount = context.significand->sz;
  size_t prefixDigitsCount = min_sz(state1, totalDigitsCount, FLT_SIG_MAX_DECIMAL_DIGITS + 1);
  uint32_t v_ui = 0;
  // Example: 7 digits and 5 prefix then i = 7 and n = 2 hence indices are [7,6,5,4,3].
  for (size_t i = totalDigitsCount, n = totalDigitsCount - prefixDigitsCount; i > n; --i) {
    v_ui = v_ui * 10 + context.significand->p[i - 1];
  }
  // Note that v_f still has to be multiplied by the exponent. 
  float v_f = (float)v_ui;
  if (context.exponent->sz > INT32_DECIMAL_DIG - 1) {
    // The maximum number of decimal digits required to represent any int32_t value.
    // Subtracting one yields the maximum number of decimal digits that can be accumulated in an int32_t
    // REGARDLESS of the values of the digits.
    return Shizu_Float32_Infinity;
  }
  int32_t exp = 0;
  for (size_t i = context.exponent->sz; i > 0; --i) {
    exp = exp * 10 + context.exponent->p[i - 1];
  }
  // Our bigints basically are of the format .w * 10^p.
  // Now, we multiply .w by 10^prefixDigitsCount.
  // We make up for that by reducing the exponent by prefixDigitsCount.
  int32_t e = exp - prefixDigitsCount;
  // v_ui contains the accumulation of the first prefixDigitCount digits of the number.
  // v_d contains the corresponding float.
  if (prefixDigitsCount < FLT_SIG_MAX_DECIMAL_DIGITS) {
    if (e == 0 || v_f == 0.f) {
      v_f = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_f : +v_f;
      Conversion_destruct(Shizu_State2_getState1(state), &context);
      return v_f;
    }

    if (e >= 0) {
      if (e <= FLT_MAX_SMALL_TEN) {
        v_f *= FLT_SMALL_10_POW[e];
        v_f = bigint_is_negative(Shizu_State2_getState1(state), context.significand) ? -v_f : +v_f;
        Conversion_destruct(Shizu_State2_getState1(state), &context);
        return v_f;
      }
      size_t slop = FLT_SIG_MAX_DECIMAL_DIGITS - prefixDigitsCount;
      if (e <= FLT_MAX_SMALL_TEN + slop) {
        // We can multiply v_d by 10^(slop) and it is still "small" and exact.
        // Then we can multiply by 10^(e-slop) with one rounding.
        v_f *= FLT_SMALL_10_POW[slop];
        v_f *= FLT_SMALL_10_POW[e - slop];
        v_f = bigint_is_negative(state1, context.significand) ? -v_f : +v_f;
        Conversion_destruct(state1, &context);
        return v_f;
      }
      // Hard case with positive exponent.
    } else {
      if (e >= -FLT_MAX_SMALL_TEN) {
        // Can get the answer in one division.
        v_f /= FLT_SMALL_10_POW[-e];
        v_f = bigint_is_negative(state1, context.significand) ? -v_f : v_f;
        Conversion_destruct(state1, &context);
        return v_f;
      }
      // Hard case with negative exponent.
    }
  } else if ((exp >= totalDigitsCount) && (totalDigitsCount + exp <= DBL_SIG_MAX_DECIMAL_DIGITS)) {
    // The first expression's reasoning eludes me.
    // 
    // The second expression basically says: we have totalDigitsCount and a positive exponent exp.
    // That is, totalDigitsCount with exp zeroes appended. If totalDigitsCount + exp digits all fit
    // into the significand of a double, then this double is certainly an exact floating point representation
    // of our number.
    // 
    // So we can compute to double, then shorten to float with one round, and get the right answer.
    //
    // First, finish accumulating digits.
    // Then convert that integer to a double, multiply
    // by the appropriate power of ten, and convert to float.
    uint64_t v_ui64 = (uint64_t)v_ui;
    for (size_t i = prefixDigitsCount; i < totalDigitsCount; ++i) {
      v_ui64 = v_ui64 * UINT64_C(10) + (uint64_t)context.significand->p[i];
    }
    double v_d = (double)v_ui64;
    e = exp - totalDigitsCount;
    v_d *= FLT_SMALL_10_POW[e];
    v_f = (float)v_d;
    v_f = bigint_is_negative(state1, context.significand) ? -v_f : +v_f;
    Conversion_destruct(state1, &context);
    return v_f;
  }
  // Harder cases:
  // The sum of digits plus exponent is greater than what we think we can do with one error.
  // Start by approximating the right answer by, naively, scaling by powers of 10.
  // Scaling uses double to avoid overflows/underflows.
  double v_d = (double)v_f;
  if (e > 0) {
    if (exp > FLT_MAX_10_EXP + 1) {
      // This is going to be infinity.
      return bigint_is_negative(state1, context.significand) ? -Shizu_Float32_Infinity : +Shizu_Float32_Infinity;
    }

    static const double DBL_BIG_10_POW[] = {
      1e16,
      1e32,
      1e64,
      1e128,
      1e256 
    };

    // DBL_SMALL_10_POW has 22 entries.
    // We mask off 15 (1111 in binary) bits which yields a value between 0 and 15 which is within the table bounds.
    if ((e & 15) != 0) {
      v_d *= DBL_SMALL_10_POW[e & 15];
    }
    if ((e >>= 4) != 0) {
      for (size_t j = 0; e > 0; j++, e >>= 1) {
        if ((e & 1) != 0) {
          v_d *= DBL_BIG_10_POW[j];
        }
      }
    }
  } else if (e < 0) {
    e = -e; // Does this overflow?
    if (exp < Shizu_Float32_MinimumDecimalExponent - 1) {
      // This is going to be zero.
      return bigint_is_negative(state1, context.significand) ? -0.f : +0.f;
    }

    if ((e & 15) != 0) {
      v_d /= DBL_SMALL_10_POW[e & 15];
    }
    if ((e >>= 4) != 0) {
      size_t j;
      for (j = 0; exp > 0; j++, e >>= 1) {
        if ((e & 1) != 0) {
          v_d *= DBL_TINY_10_POW[j];
        }
      }
    }
  }

  v_f = clamp_f32(state, (Shizu_Float32)v_d, Shizu_Float32_Minimum, Shizu_Float32_Maximum);

  // v_f is now approximately the result.
  // The hard part is adjusting it, by comparison with bigint_t arithmetic.
  // Formulate the EXACT big-number result as bigD0 * 10^exp
  if (totalDigitsCount > Shizu_Float32_MaximumInputDecimalDigits) {
    // We stop considering digits that are deemed "insignificant".
    size_t newTotalDigitsCount = Shizu_Float64_MaximumInputDecimalDigits + 1;
    size_t delta = totalDigitsCount - newTotalDigitsCount;
    totalDigitsCount = newTotalDigitsCount;
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      // Basically we remove delta most insignificand digits and replace them by a 1.
      bigint_in_situ_div_p10(state1, context.significand, delta);
      bigint_in_situ_mul_p10(state1, context.significand, 1);
      bigint_in_situ_add_u8(state1, context.significand, 1);
      Shizu_State2_popJumpTarget(state);
    } else {
      Conversion_destruct(state1, &context);
      Shizu_State2_jump(state);
    }
  }
  e = exp - totalDigitsCount;

  // The exact result is represented by "bigd0 * 10^e". 
  bigint_t* bigd0 = NULL;

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    bigd0 = bigint_from_u32(state1, v_ui);
    size_t pow = 0;
    for (size_t i = prefixDigitsCount; i < totalDigitsCount; ++i) {
      bigint_in_situ_mul_p10(state1, bigd0, 1);
      bigint_in_situ_add_u8(state1, bigd0, context.significand->p[i]);
    }
    Shizu_State1_popJumpTarget(state1);
  } else {
    Shizu_State1_popJumpTarget(state1);
  }

  uint32_t ieeeBits = Shizu_f32ToU32(state1, v_f); // IEEE-754 bits of float candidate
  const int32_t b5 = max_s32(state1, 0, -e);       // powers of 5 in bigb, value is not modified inside correctionLoop
  const int32_t d5 = max_s32(state1, 0, +e);       // powers of 5 in bigd, value is not modified inside correctionLoop
  bigint_in_situ_mul(state1, bigd0, bigint_p5(state1, d5)); /// FIXME: Leak.
  bigint_t* bigd = NULL;
  int prevd2 = 0;
  while (true) {
    // The 8 bits of the exponent.
    int32_t ieeeExponentBits = (ieeeBits & Shizu_Float32_ExponentMask) >> Shizu_Float32_ExponentShift;
    // The 23 bits of the significand.
    int32_t ieeeSignificandBits = (ieeeBits & Shizu_Float32_SignificandMask) >> Shizu_Float32_SignificandShift;
    /// ieeeBits is not NaN, Infinity, or Zero at the point.
    if (((int32_t)ieeeExponentBits) > 0) {
      // The MSB of ieeeExponentBits is zero and other bits are non-zero.
      // We hava a normalized fpv "(-1)^sign x 2^(exponent - 127) x 1.fraction"
      // We need to add the implicit one to our binary representation.
      ieeeSignificandBits |= 1 << Shizu_Float32_ExponentShift;
    } else {
      // All bits of ieeeExponentBits are zero.
      // We have a denormalized fpv "(-1)^sign x 2^(-126) x 0.fraction".
      // Normalize the denormalized number.
      size_t leadingZeroesCount = Shizu_countLeadingZeroesU32(state1, ieeeSignificandBits);
      size_t shift = leadingZeroesCount - (31 - Shizu_Float32_ExponentShift);
      ieeeSignificandBits <<= shift;  // We multiply the significand by 2^shift
      ieeeExponentBits = 1 - shift;   // The exponent was 0. Make up for the multiplication of the significand by 2^(1 - shift).
    }
    ieeeExponentBits -= Shizu_Float32_ExponentBias;
    size_t trailingZeroesCount = Shizu_countTrailingZeroesU32(state1, ieeeSignificandBits);
    ieeeSignificandBits >>= trailingZeroesCount;

    const int32_t bigIntExp = ieeeExponentBits - Shizu_Float32_ExponentShift + trailingZeroesCount;
    const int32_t bigIntNBits = Shizu_Float32_ExponentShift + 1 - trailingZeroesCount;

    int32_t b2 = b5; // powers of 2 in bigB
    int32_t d2 = d5; // powers of 2 in bigD

    int32_t ulp2;   // powers of 2 in halfUlp.
    if (bigIntExp >= 0) {
      b2 += bigIntExp;
    } else {
      d2 -= bigIntExp;
    }
    ulp2 = b2;
    // shift bigb and bigd left by a number s. t.
    // halfUlp is still an integer.
    int hulpbias;
    if (ieeeExponentBits <= -Shizu_Float32_ExponentBias) {
        // This is going to be a denormalized number (if not actually zero).
        // Half an ULP is at 2^-(Shizu_Float32_ExponentBias + Shizu_Float32_ExponentShift + 1)
      hulpbias = ieeeExponentBits + trailingZeroesCount + Shizu_Float32_ExponentBias;
    } else {
      hulpbias = 1 + trailingZeroesCount;
    }
    b2 += hulpbias;
    d2 += hulpbias;
    int32_t common2 = min_s32(state1, b2, min_s32(Shizu_State2_getState1(state), d2, ulp2));
    b2 -= common2;
    d2 -= common2;
    ulp2 -= common2;

    bigint_t* bigb = bigint_mul_p5_p2(state1, ieeeSignificandBits, b5, b2);
    if (bigd == NULL || prevd2 != d2) {
      bigd = bigint_mul(state1, bigd0, bigint_p2(state1, d2));
      prevd2 = d2;
    }
    // to recap:
    // bigb is the scaled-big-int version of our floating-point candidate.
    // bigd is the scaled-big-int version of the exact value as we understand it.
    // halfUlp is 1/2 an ulp of bigb, except for special cases of exact powers of 2
    //
    // the plan is to compare bigb with bigd, and if the difference is less than halfUlp, then we're satisfied.
    // Otherwise, use the ratio of difference to halfUlp to calculate a fudge factor to add to the floating value, then go 'round again.

    bigint_t*diff = NULL;
    int8_t cmp = bigint_compare(state1, bigb, bigd);
    bool overvalue = false;
    if (cmp < 0) {
      overvalue = true; // candidate is too big
      diff = bigint_subtract(state1, bigb, bigd);
      bigint_free(state1, bigb);
      bigb = NULL;
      if (bigIntNBits == 1 && bigIntExp > Shizu_Float32_ExponentBias) {
        // Candidate is a normalized exact power of 2 and is too big.
        // That is, it is larger than Shizu_Float32_MinNormal.
        // We will be subtracting.
        // For our purposes, ulp is the ulp of the next smaller range.
        ulp2 -= 1;
        if (ulp2 < 0) {
          // Cannot de-scale ulp this far.
          // Must scale diff in other direction.
          ulp2 = 0;
          bigint_in_situ_mul_p2(state1, diff, 1);
        }
      }
    } else if (cmp > 0) {
      overvalue = false; // candidate is too small
      diff = bigint_subtract(state1, bigd, bigb);
      bigint_free(state1, bigb);
    } else {
      // The candidate is exactly right.
      // This happens with surprising frequency.
      break;
    }
    cmp = bigint_compare_p5_p2(state1, diff, b5, ulp2);
    if (cmp < 0) {
      // Difference is small.
      // This is close enough.
      break;
    } else if (cmp == 0) {
      // Difference is exactly half an ULP.
      // Round to some other value maybe, then finish.
      if ((ieeeBits & 1) != 0) { // half ties to even
        ieeeBits += overvalue ? -1 : 1; // nextDown or nextUp
      }
      break;
    } else {
      // Difference is non-trivial.
      // Could scale addend by ratio of difference to halfUlp here,
      // if we bothered to compute that difference.
      // Most of the time ( I hope ) it is about 1 anyway.
      ieeeBits += overvalue ? -1 : 1; // nextDown or nextUp
      if (ieeeBits == 0 || ieeeBits == Shizu_Float32_ExponentMask) { // 0.f or +Shizu_Float32_Infinity.
        break; // Fell off end of range.
      }
      continue; // try again.
    }
  }
  if (bigint_is_negative(state1, context.significand)) {
    ieeeBits |= Shizu_Float32_SignMask;
  }
  // Basically we want to take v_ui, the digits from prefixDigitsCount to totalDigitsCount.
  Conversion_destruct(state1, &context);
  return 0.f;
}

#if defined(Shizu_Configuration_WithTests)

void
Shizu_Operations_StringToFloat_Version1_tests
  (
    Shizu_State2* state
  )
{
  static_assert(-307 == DBL_MIN_10_EXP, "<error>"); // -307
  static_assert(+308 == DBL_MAX_10_EXP, "<error>"); // +308
}

#endif
