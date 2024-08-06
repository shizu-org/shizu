#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/Utilities/BigInt/Include.h"

#include "Shizu/Gc/Include.h"

#if defined(Shizu_Configuration_WithTests)
#include <string.h>
#endif

// Table entry at index i contains the 5^i for 0 <= i <= 27.
static uint64_t const small_power_of_5[] = {
  UINT64_C(1), // 5^0
  UINT64_C(5), // 5^1
  UINT64_C(25), // 5^2
  UINT64_C(125), // 5^3
  UINT64_C(625), // 5^4
  UINT64_C(3125), // 5^5
  UINT64_C(15625), // 5^6
  UINT64_C(78125), // 5^7
  UINT64_C(390625), // 5^8
  UINT64_C(1953125), // 5^9
  UINT64_C(9765625), // 5^10
  UINT64_C(48828125), // 5^11
  UINT64_C(244140625), // 5^12
  UINT64_C(1220703125), // 5^13
  UINT64_C(6103515625), // 5^14
  UINT64_C(30517578125), // 5^15
  UINT64_C(152587890625), // 5^16
  UINT64_C(762939453125), // 5^17
  UINT64_C(3814697265625), // 5^18
  UINT64_C(19073486328125), // 5^19
  UINT64_C(95367431640625), // 5^20
  UINT64_C(476837158203125), // 5^21
  UINT64_C(2384185791015625), // 5^22
  UINT64_C(11920928955078125), // 5^23
  UINT64_C(59604644775390625), // 5^24
  UINT64_C(298023223876953125), // 5^25
  UINT64_C(1490116119384765625), // 5^26
  UINT64_C(7450580596923828125), // 5^27
};

static_assert(sizeof(small_power_of_5) / sizeof(uint64_t) == 28, "<internal error>");

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct bigint_globals_t {
  int dummy;
} bigint_globals_t;

void
bigint_init
  (
    Shizu_State1* state
  )
{
  bigint_globals_t* p = NULL;
  if (Shizu_Gcx_Pls_create("<bigint>", strlen("<bigint>"), sizeof(bigint_globals_t), &p)) {
    Shizu_State1_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State1_jump(state);
  }
  // Initialize!
}

bool
bigint_is_initialize
  (
    Shizu_State1* state
  )
{ 
  bool exists;
  if (Shizu_Gcx_Pls_exists("<bigint>", strlen("<bigint>"), &exists)) {
    Shizu_State1_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State1_jump(state);
  }
  return exists;
}

void
bigint_uninit
  (
    Shizu_State1* state
  )
{
  bigint_globals_t* p = NULL;
  if (Shizu_Gcx_Pls_get("<bigint>", strlen("<bigint>"), &p)) {
    Shizu_State1_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State1_jump(state);
  }
  // Uninitialize!
  Shizu_Gcx_Pls_destroy("<bigint>", strlen("<bigint>"));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(Shizu_Configuration_WithTests)

static void
bigint_test_add
  (
    Shizu_State1* state
  )
{
#define TEST(SUFFIX, X, Y, MAGNITUDE, SIGN) \
  { \
    bigint_t* x = NULL, *y = NULL, * z = NULL; \
    Shizu_JumpTarget jumpTarget; \
    Shizu_State1_pushJumpTarget(state, &jumpTarget); \
    if (!setjmp(jumpTarget.environment)) { \
      x = bigint_from_##SUFFIX(state, X); \
      y = bigint_from_##SUFFIX(state, Y); \
      z = bigint_add(state, x, y); \
      char const* expected = MAGNITUDE; \
      if (strlen(expected) != z->sz) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      if (z->sign != SIGN) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      for (size_t i = 0, j = strlen(expected); i < z->sz; ++i, --j) { \
        if ((expected[j - 1] - '0') != z->p[i]) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
      } \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_popJumpTarget(state); \
    } else { \
      Shizu_State1_popJumpTarget(state); \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_jump(state); \
    } \
  }

  static const char* MAX_INT64_STR  =  "9223372036854775808";
  static const char* MAX_UINT8_STR  =                  "255";
  static const char* MAX_UINT16_STR =                "65535";
  static const char* MAX_UINT32_STR =           "4294967295";
  static const char* MAX_UINT64_STR = "18446744073709551615";


  TEST(i64, INT64_MIN, 0, MAX_INT64_STR, -1);

  TEST(i8,  0, 0, "0", 0);
  TEST(i16, 0, 0, "0", 0);
  TEST(i32, 0, 0, "0", 0);
  TEST(i64, 0, 0, "0", 0);
  TEST(u8,  0, 0, "0", 0);
  TEST(u16, 0, 0, "0", 0);
  TEST(u32, 0, 0, "0", 0);
  TEST(u64, 0, 0, "0", 0);

  TEST(i64, +1,  0, "1", 1);
  TEST(i64,  0, +1, "1", 1);

  TEST(i64, -1, 0, "1", -1);
  TEST(i64,  0, -1, "1", -1);

  TEST(i64, +1, -2, "1", -1);
  TEST(i64, -2, +1, "1", -1);

  // In case you are wondering why the next four tests are there.
  // They were in the tests of the old implementation so we decided to keep them.
  TEST(i64, 0, 5, "5", +1);
  TEST(i64, 5, 0, "5", +1);
  TEST(i64, 7, 5, "12", +1);
  TEST(i64, 5, 7, "12", +1);

  TEST(i64, +1998691, +9999991, "11998682", 1);
  TEST(i64, +9999991, +1998691, "11998682", 1);

  TEST(i64, +1998691, -9999991, "8001300", -1);
  TEST(i64, -9999991, +1998691, "8001300", -1);

  TEST(i64, INT32_MIN, 0, "2147483648", -1);
  TEST(i64, INT32_MAX, 0, "2147483647", +1);

  TEST(u8,  UINT8_MAX,  0, MAX_UINT8_STR, +1);
  TEST(u16, UINT16_MAX, 0, MAX_UINT16_STR, +1);
  TEST(u32, UINT32_MAX, 0, MAX_UINT32_STR, +1);
  TEST(u64, UINT64_MAX, 0, MAX_UINT64_STR, +1);

#undef TEST
}

static void
bigint_test_from_integer
  (
    Shizu_State1* state
  )
{
#define TEST(SUFFIX, INPUT, MAGNITUDE, SIGN) \
  { \
    bigint_t* x = NULL; \
    Shizu_JumpTarget jumpTarget; \
    Shizu_State1_pushJumpTarget(state, &jumpTarget); \
    if (!setjmp(jumpTarget.environment)) { \
      x = bigint_from_##SUFFIX(state, INPUT); \
      char expected[] = MAGNITUDE; \
      if (strlen(expected) != x->sz) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      if (x->sign != SIGN) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      for (size_t i = 0, j = strlen(expected); i < x->sz; ++i, --j) { \
        if ((expected[j - 1] - '0') != x->p[i]) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_popJumpTarget(state); \
    } else { \
      Shizu_State1_popJumpTarget(state); \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_jump(state); \
    } \
  }

  TEST(i64, 0, "0", 0);
  TEST(i64, Shizu_Integer64_Minimum, "9223372036854775808", -1);
  TEST(i64, Shizu_Integer64_Maximum, "9223372036854775807", +1);

  TEST(i32, 0, "0", 0);
  TEST(i32, Shizu_Integer32_Minimum, "2147483648", -1);
  TEST(i32, Shizu_Integer32_Maximum, "2147483647", +1);

#undef TEST
}

static void
bigint_test_in_situ_div_p10
  (
    Shizu_State1* state
  )
{
#define TEST(X,P,MAGNITUDE,SIGN) \
    { \
      bigint_t* x = NULL; \
      Shizu_JumpTarget jumpTarget; \
      Shizu_State1_pushJumpTarget(state, &jumpTarget); \
      if (!setjmp(jumpTarget.environment)) { \
        x = bigint_from_i64(state, X); \
        bigint_in_situ_div_p10(state, x, P); \
        char expected[] = MAGNITUDE; \
        if (strlen(expected) != x->sz) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        if (x->sign != SIGN) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        for (size_t i = 0, j = strlen(expected); i < x->sz; ++i, --j) { \
          if ((expected[j - 1] - '0') != x->p[i]) { \
            Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
            Shizu_State1_jump(state); \
          } \
        } \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_popJumpTarget(state); \
      } else { \
        Shizu_State1_popJumpTarget(state); \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_jump(state); \
      } \
    }

  TEST(0, 0, "0", 0);
  TEST(0, 1, "0", 0);
  TEST(0, 2, "0", 0);

  TEST(1, 1, "0", 0);
  TEST(7, 1, "0", 0);

  TEST(-90, 0, "90", -1);
  TEST(-91, 0, "91", -1);
  TEST(-91, 1, "9", -1);
  TEST(-900, 1, "90", -1);
  TEST(-901, 1, "90", -1);

  // 983 is the largest 3 digit prime with all its digits different.
  TEST(983, 0, "983", 1);
  TEST(983, 1, "98", 1);
  TEST(983, 2, "9", 1);
  TEST(983, 3, "0", 0);

#undef TEST
}

static void
bigint_test_in_situ_mul_p10
  (
    Shizu_State1* state
  )
{
#define TEST(X,P,MAGNITUDE,SIGN) \
    { \
      bigint_t* x = NULL; \
      Shizu_JumpTarget jumpTarget; \
      Shizu_State1_pushJumpTarget(state, &jumpTarget); \
      if (!setjmp(jumpTarget.environment)) { \
        x = bigint_from_i64(state, X); \
        bigint_in_situ_mul_p10(state, x, P); \
        char expected[] = MAGNITUDE; \
        if (strlen(expected) != x->sz) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        if (x->sign != SIGN) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        for (size_t i = 0, j = strlen(expected); i < x->sz; ++i, --j) { \
          if ((expected[j - 1] - '0') != x->p[i]) { \
            Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
            Shizu_State1_jump(state); \
          } \
        } \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_popJumpTarget(state); \
      } else { \
        Shizu_State1_popJumpTarget(state); \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_jump(state); \
      } \
    }

  TEST(-9, 0, "9", -1);
  TEST(-9, 1, "90", -1);
  TEST(-9, 2, "900", -1);

#undef TEST
}

static void
bigint_test_in_situ_mul_p2
  (
    Shizu_State1* state
  )
{
#define TEST(X,P,MAGNITUDE,SIGN) \
    { \
      bigint_t* x = NULL; \
      Shizu_JumpTarget jumpTarget; \
      Shizu_State1_pushJumpTarget(state, &jumpTarget); \
      if (!setjmp(jumpTarget.environment)) { \
        x = bigint_from_i64(state, X); \
        bigint_in_situ_mul_p2(state, x, P); \
        char expected[] = MAGNITUDE; \
        if (strlen(expected) != x->sz) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        if (x->sign != SIGN) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
        for (size_t i = 0, j = strlen(expected); i < x->sz; ++i, --j) { \
          if ((expected[j - 1] - '0') != x->p[i]) { \
            Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
            Shizu_State1_jump(state); \
          } \
        } \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_popJumpTarget(state); \
      } else { \
        Shizu_State1_popJumpTarget(state); \
        if (x) { \
          bigint_free(state, x); \
          x = NULL; \
        } \
        Shizu_State1_jump(state); \
      } \
    }

  TEST(-9, 0, "9", -1);
  TEST(-9, 1, "18", -1);
  TEST(-9, 2, "36", -1);

#undef TEST
}

static void
bigint_test_mul
  (
    Shizu_State1* state
  )
{
#define TEST(X, Y, MAGNITUDE, SIGN) \
  { \
    bigint_t* x = NULL, *y = NULL, * z = NULL; \
    Shizu_JumpTarget jumpTarget; \
    Shizu_State1_pushJumpTarget(state, &jumpTarget); \
    if (!setjmp(jumpTarget.environment)) { \
      x = bigint_from_i64(state, X); \
      y = bigint_from_i64(state, Y); \
      z = bigint_mul(state, x, y); \
      char expected[] = MAGNITUDE; \
      if (strlen(expected) != z->sz) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      if (z->sign != SIGN) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      for (size_t i = 0, j = strlen(expected); i < z->sz; ++i, --j) { \
        if ((expected[j - 1] - '0') != z->p[i]) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
      } \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_popJumpTarget(state); \
    } else { \
      Shizu_State1_popJumpTarget(state); \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_jump(state); \
    } \
  }

  TEST(0, 0, "0", 0);
  TEST(INT64_MIN, 0, "0", 0);

  TEST(+1, 0, "0", 0);
  TEST(0, +1, "0", 0);

  TEST(-1, 0, "0", 0);
  TEST(0, -1, "0", 0);

  TEST(+1, -2, "2", -1);
  TEST(-2, +1, "2", -1);

  TEST(+7, +97, "679", 1);
  TEST(+97, +7, "679", 1);

  TEST(+7, -97, "679", -1);
  TEST(-97, +7, "679", -1);

  TEST(-5, -7, "35", +1)
  TEST(+5, -7, "35", -1)
  TEST(-5, +7, "35", -1)

#undef TEST
}

static void
bigint_test_negate
  (
    Shizu_State1* state
  )
{
#define TEST(X, MAGNITUDE, SIGN) \
  { \
    bigint_t* x = NULL, *y = NULL; \
    Shizu_JumpTarget jumpTarget; \
    Shizu_State1_pushJumpTarget(state, &jumpTarget); \
    if (!setjmp(jumpTarget.environment)) { \
      x = bigint_from_i64(state, X); \
      y = bigint_negate(state, x); \
      char expected[] = MAGNITUDE; \
      if (strlen(expected) != y->sz) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      if (y->sign != SIGN) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      for (size_t i = 0, j = strlen(expected); i < y->sz; ++i, --j) { \
        if ((expected[j - 1] - '0') != y->p[i]) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_popJumpTarget(state); \
    } else { \
      Shizu_State1_popJumpTarget(state); \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_jump(state); \
    } \
  }

  TEST(INT64_MIN, "9223372036854775808", +1);
  TEST(INT64_MAX, "9223372036854775807", -1);
  TEST(0, "0", 0);
  TEST(INT32_MIN, "2147483648", +1);
  TEST(INT32_MAX, "2147483647", -1);

#undef TEST
}

static void
bigint_test_subtract
  (
    Shizu_State1* state
  )
{
#define TEST(X, Y, MAGNITUDE, SIGN) \
  { \
    bigint_t* x = NULL, *y = NULL, *z = NULL; \
    Shizu_JumpTarget jumpTarget; \
    Shizu_State1_pushJumpTarget(state, &jumpTarget); \
    if (!setjmp(jumpTarget.environment)) { \
      x = bigint_from_i64(state, X); \
      y = bigint_from_i64(state, Y); \
      z = bigint_subtract(state, x, y); \
      char expected[] = MAGNITUDE; \
      if (strlen(expected) != z->sz) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      if (z->sign != SIGN) { \
        Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
        Shizu_State1_jump(state); \
      } \
      for (size_t i = 0, j = strlen(expected); i < z->sz; ++i, --j) { \
        if ((expected[j - 1] - '0') != z->p[i]) { \
          Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed); \
          Shizu_State1_jump(state); \
        } \
      } \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_popJumpTarget(state); \
    } else { \
      Shizu_State1_popJumpTarget(state); \
      if (z) { \
        bigint_free(state, z); \
        z = NULL; \
      } \
      if (y) { \
        bigint_free(state, y); \
        y = NULL; \
      } \
      if (x) { \
        bigint_free(state, x); \
        x = NULL; \
      } \
      Shizu_State1_jump(state); \
    } \
  }

  TEST(0,  0, "0", 0);
  TEST(0,  5, "5", -1);
  TEST(5, -5, "10", +1);
  TEST(5, 7, "2", -1);
  TEST(2147483648, 0, "2147483648", +1);
  TEST(0, 2147483648, "2147483648", -1);

#undef TEST
}

static void
bigint_test_tables
  (
    Shizu_State1* state
  )
{
  // Check if the entries in the "small_power_of_5" table are sane.
  if (small_power_of_5[0] != 1) {
    Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state);
  }
  uint64_t x = UINT64_C(5);
  for (size_t i = 1, n = sizeof(small_power_of_5) / sizeof(uint64_t); i < n; ++i) {
    if (small_power_of_5[i] != x) {
      Shizu_State1_setStatus(state, Shizu_Status_EnvironmentFailed);
      Shizu_State1_jump(state);
    }
    x *= UINT64_C(5);
  }
}

void
bigint_tests
  (
    Shizu_State1* state
  )
{
  bigint_test_add(state);
  bigint_test_from_integer(state);
  bigint_test_in_situ_div_p10(state);
  bigint_test_in_situ_mul_p10(state);
  bigint_test_in_situ_mul_p2(state);
  bigint_test_subtract(state);
  bigint_test_mul(state);
  bigint_test_negate(state);
  bigint_test_tables(state);
}

#endif
