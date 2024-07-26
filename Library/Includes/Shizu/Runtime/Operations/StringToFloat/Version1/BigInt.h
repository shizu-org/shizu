#if !defined(SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_BIGINT_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_BIGINT_H_INCLUDED

#include "Shizu/Runtime/Value.h"
#include "Shizu/Runtime/State1.h"
#include <malloc.h>

/// The least significant digit is stored at index 0.
/// The most significant digit is stored at index sz - 1.
#define BIGINT_CFG_ORDER_LSD_TO_MSD (1)
#define BIGINT_CFG_ORDER_MSD_TO_LSD (2)

/// The most significant digit is stored at index 0.
/// The least significant digit is stored at index sz - 1.
#define BIGINT_CFG_ORDER BIGINT_CFG_ORDER_LSD_TO_MSD

typedef struct bigint_t {
  int8_t sign; // -1 negative, +1 positive. 0 if the bigint is zero.
  // Pointer to an array of cp uint8_t values.
  // First sz uint8_t values are digits base 10.
  // The least signficant digit is stored at index 0 and the most significant at index sz - 1 if BIGINT_CFG_ORDER is BIGINT_CFG_ORDER_LSD_TO_MSG.
  // The most significant digit is stored at index 0 and the least significant at index sz - 1 if BIGINT_CFG_ORDER is BIGINT_CFG_ORDER_MSB_TO_LSB.
  // If the bigint is zero, then p[0] = 0 and sz = 1 and sign = 0.
  // Otherwise p[j] != 0 where j is the index of the most significant digit.
  uint8_t* p;
  size_t sz, cp;
} bigint_t;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Initialize the cache.
/// @error Shizu_Status_OperationInvalid the cache is initialized
void
bigint_init
  (
    Shizu_State1* state
  );

/// @brief Get if the cache is initialized.
/// @return @a true if the cache is initialized. @a false otherwise.
bool
bigint_is_initialize
  (
    Shizu_State1* state
  );

/// @brief Uninitialize the cache.
/// @error Shizu_Status_OperationInvalid the cache is uninitialized
void
bigint_uninit
  (
    Shizu_State1* state
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline bigint_t*
bigint_add
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  );

static inline void
bigint_in_situ_add_u8
  (
    Shizu_State1* state,
    bigint_t* x,
    uint8_t v
  );

static inline bigint_t*
bigint_add_u8
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint8_t v
  );

static inline void
bigint_in_situ_add_u16
  (
    Shizu_State1* state,
    bigint_t* x,
    uint16_t v
  );

static inline bigint_t*
bigint_add_u16
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint16_t v
  );

static inline void
bigint_in_situ_add_u32
  (
    Shizu_State1* state,
    bigint_t* x,
    uint32_t v
  );

static inline bigint_t*
bigint_add_u32
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint32_t v
  );

static inline void
bigint_in_situ_add_u64
  (
    Shizu_State1* state,
    bigint_t* x,
    uint64_t v
  );

static inline bigint_t*
bigint_add_u64
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint64_t v
  );

static inline bigint_t*
bigint_clone
  (
    Shizu_State1* state,
    bigint_t const* x
  );

static inline int8_t
bigint_compare
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  );

static inline int8_t
bigint_compare_magnitudes
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  );

// Compare x with 5^b5 * 2^b2
static inline int32_t
bigint_compare_pow5_pow2
  (
    Shizu_State1* state,
    bigint_t const* x,
    int32_t b5,
    int32_t b2
  );

static inline void
bigint_ensure_free_cp
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t required_free_cp
  );

static inline void
bigint_free
  (
    Shizu_State1* state,
    bigint_t* x
  );

/// In-place divide a bigint with 10^n.
static inline void
bigint_in_situ_div_10
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t n
  );

/// In-place multiply a bigint with 10^n.
static inline void
bigint_in_situ_mul_10
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t n
  );

// In-place multiply a bigint with 2^n.
static inline void
bigint_in_situ_mul_2
  (
    Shizu_State1* state,
    bigint_t* x,
    uint64_t n
  );

static inline void
bigint_increase_cp
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t additional_cp
  );

static inline void
bigint_in_situ_mul
  (
    Shizu_State1* state,
    bigint_t* x,
    bigint_t const* y
  );

static inline bigint_t*
bigint_mul
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  );

// Create bigint v * 5^b5 * 2^b2.
// TODO: Should be renamed.
static inline bigint_t*
bigint_mul_pow_5_pow_2
  (
    Shizu_State1* state,
    int32_t v,
    int32_t b5,
    int32_t b2
  );

static inline bigint_t*
bigint_negate
  (
    Shizu_State1* state,
    bigint_t const* x
  );

// Compute 2^n, 0 <= n <= UINT64_MAX.
static bigint_t*
bigint_pow2
  (
    Shizu_State1* state,
    uint64_t n
  );

// Compute 5^n, 0 <= n <= UINT64_MAX.
static bigint_t*
bigint_pow5
  (
    Shizu_State1* state,
    uint64_t n
  );

// Create a bigint representing the specified uint8_t value.
static inline bigint_t*
bigint_from_u8
  (
    Shizu_State1* state,
    uint8_t v
  );

// Create a bigint representing the specified uint16_t value.
static inline bigint_t*
bigint_from_u16
  (
    Shizu_State1* state,
    uint16_t v
  );

// Create a bigint representing the specified uint32_t value.
static inline bigint_t*
bigint_from_u32
  (
    Shizu_State1* state,
    uint64_t v
  );

// Create a bigint representing the specified uint64_t value.
static inline bigint_t*
bigint_from_u64
  (
    Shizu_State1* state,
    uint64_t v
  );

static inline bigint_t*
bigint_subtract
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  );
  
static inline bool
bigint_is_zero
  (
    Shizu_State1* state,
    bigint_t const* x
  );

static bool
bigint_is_positive
  (
    Shizu_State1* state,
    bigint_t const* x
  );

static bool
bigint_is_negative
  (
    Shizu_State1* state,
    bigint_t const* x
  );
  
// no error
static void
_bigint_trim_leading_zeroes
  (
    Shizu_State1* state,
    bigint_t* x
  );

// no error
static void
_bigint_swap
  (
    Shizu_State1* state,
    bigint_t* x,
    bigint_t* y
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline bigint_t*
bigint_add
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  )
{
  if (x->sign == 0) {
    return bigint_clone(state, y);
  }
  if (y->sign == 0) {
    return bigint_clone(state, x);
  }
  if (x->sign == y->sign) {
    bigint_t const* a = x, *b = y;
    // Signs are equal and neither x nor y is zero.
    // Swap x and y if x is shorter than y.
    if (a->sz < b->sz) {
      bigint_t const* t = a;
      a = b;
      b = t;
    }
    size_t cn = a->sz;
    uint8_t* cp = malloc(sizeof(uint8_t) * cn);
    if (!cp) {
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    bool carry = false;
    uint8_t const* ap = a->p;
    uint8_t const* bp = b->p;
    size_t ai = 0;
    size_t an = a->sz;
    size_t bi = 0;
    size_t bn = b->sz;
    // Add common part.
    while (bi < bn) {
      uint8_t sum = ap[ai] + bp[bi] + (carry ? 1 : 0);
      cp[ai] = sum % 10;
      carry = sum / 10;
      ai++;
      bi++;
    }
    // Add part specific to the longer number while propagating carry.
    while (ai < an && carry) {
      uint8_t sum = ap[ai] + 1;
      cp[ai] = sum % 10;
      carry = sum / 10;
      ai++;
    }
    // Add part specific to the longer number.
    while (ai < an) {
      cp[ai] = ap[ai];
    }
    // Handle remaining carry (if any).
    if (carry) {
      // @todo Check for overflow zn + 1.
      uint8_t* t = realloc(cp, sizeof(uint8_t) * (cn + 1));
      if (!t) {
        free(cp);
        Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
        Shizu_State1_jump(state);
      }
      cp = t;
      cp[ai] = carry;
      cn++;
    }
    bigint_t* c = malloc(sizeof(bigint_t));
    if (!c) {
      free(cp);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    c->p = cp;
    c->sz = cn;
    c->cp = cn;
    c->sign = a->sign;
    return c;
  } else {
    bigint_t const* a = x, *b = y;
    // Signs are not and neither x nor y is zero.
    // Swap x and y if the magnitude of x is smaller than the magnitude of y.
    int32_t cmp = bigint_compare_magnitudes(state, a, b);
    if (cmp < 0) {
      bigint_t const* t = a;
      a = b;
      b = t;
    }
    size_t cn = a->sz;
    uint8_t* cp = malloc(sizeof(uint8_t) * cn);
    if (!cp) {
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    bool borrow = false;
    uint8_t const* ap = a->p;
    uint8_t const* bp = b->p;
    size_t ai = 0;
    size_t bi = 0;
    size_t an = a->sz;
    size_t bn = b->sz;
    // Subtract common part.
    while (bi < bn) {
      uint8_t u = ap[ai];
      uint8_t v = bp[bi] + (borrow ? 1 : 0);
      borrow = u < v;
      uint8_t difference = borrow ? v - u : u - v;
      cp[ai] = difference;
      ai++;
      bi++;
    }
    // Subtract part specific to the longer number while propagating borrow.
    while (ai < an && borrow) {
      uint8_t u = ap[ai];
      uint8_t v = 1;
      borrow = u < v;
      uint8_t difference = (u < v) ? v - u : u - v;
      cp[ai] = difference;
      ai++;
      //yi++;
    }
    // Subtract part specific to the longer number.
    while (ai < an) {
      cp[ai] = ap[ai];
      ai++;
      //yi++
    }
    bigint_t* c = malloc(sizeof(bigint_t));
    if (!c) {
      free(cp);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    c->p = cp;
    c->sz = cn;
    c->cp = cn;
    // The sign of the greater (in terms of magnitude) of the two numbers determines the result.
    if (cmp < 0) {
      c->sign = y->sign;
    } else {
      c->sign = x->sign;
    }
    return c;
  }
}

static inline void
bigint_in_situ_add_u8
  (
    Shizu_State1* state,
    bigint_t* x,
    uint8_t v
  )
{ bigint_in_situ_add_u64(state, x, v); }

static inline bigint_t*
bigint_add_u8
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint8_t v
  )
{ return bigint_add_u64(state, x, v); }

static inline void
bigint_in_situ_add_u16
  (
    Shizu_State1* state,
    bigint_t* x,
    uint16_t v
  )
{ bigint_in_situ_add_u16(state, x, v); }

static inline bigint_t*
bigint_add_u16
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint16_t v
  )
{ return bigint_add_u64(state, x, v); }

static inline void
bigint_in_situ_add_u32
  (
    Shizu_State1* state,
    bigint_t* x,
    uint32_t v
  )
{ bigint_in_situ_add_u64(state, x, v); }

static inline bigint_t*
bigint_add_u32
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint32_t v
  )
{ return bigint_add_u64(state, x, v); }

static inline void
bigint_in_situ_add_u64
  (
    Shizu_State1* state,
    bigint_t* x,
    uint64_t v
  )
{
  bigint_t* temporary = bigint_add_u64(state, x, v);
  _bigint_swap(state, x, temporary);
  bigint_free(state, temporary);
}

static inline bigint_t*
bigint_add_u64
  (
    Shizu_State1* state,
    bigint_t const* x,
    uint64_t v
  )
{
  bigint_t* a = NULL,
          * b = NULL ,
          * c = NULL;
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    a = bigint_clone(state, x);
    b = bigint_from_u64(state, v);
    c = bigint_add(state, a, b);
    bigint_free(state, b);
    b = NULL;
    bigint_free(state, a);
    a = NULL;
    Shizu_State1_popJumpTarget(state);
    return c;
  } else {
    if (c) {
      bigint_free(state, c);
      c = NULL;
    }
    if (b) {
      bigint_free(state, b);
      b = NULL;
    }
    if (a) {
      bigint_free(state, a);
      a = NULL;
    }
    Shizu_State1_jump(state);
  }
}

static inline int8_t
bigint_compare
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  )
{
  if (x->sign == y->sign) {
    if (0 == x->sign) {
      return 0;
    } else {
      return bigint_compare_magnitudes(state, x, y);
    }
  } else {
    if (-1 == x->sign) {
      return -1;
    } else if (+1 == x->sign) {
      return +1;
    }
    // |x| = 0.
    // The following could be optimized to just return y->sign.
    if (-1 == y->sign) {
      return -1;
    } else if (+1 == y->sign) {
      return +1;
    }
    return 0;
  }
}

static inline int8_t
bigint_compare_magnitudes
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  )
{
  if (x->sz < y->sz) {
    return -1;
  } else if (x->sz > y->sz) {
    return +1;
  } else {
  #if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
      // Because the most significant digit is at sz - 1.
    for (size_t i = x->sz; i > 0; --i) {
      if (x->p[i - 1] < y->p[i - 1]) {
        return -1;
      } else if (x->p[i - 1] > y->p[i - 1]) {
        return +1;
      }
    }
  #else
  #error("not yet implemented")
  #endif
    return 0;
  }
}

static inline int32_t
bigint_compare_pow5_pow2
  (
    Shizu_State1* state,
    bigint_t const* x,
    int32_t b5,
    int32_t b2
  )
{
  bigint_t* y = NULL;
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    y = bigint_mul_pow_5_pow_2(state, 1, b5, b2);
    int32_t cmp = bigint_compare(state, x, y);
    bigint_free(state, y);
    y = NULL;
    Shizu_State1_popJumpTarget(state);
    return cmp;
  } else {
    if (y) {
      bigint_free(state, y);
      y = NULL;
    }
    Shizu_State1_popJumpTarget(state);
    Shizu_State1_jump(state);
  }
}

static inline void
bigint_free
  (
    Shizu_State1* state,
    bigint_t* x
  )
{
  free(x->p);
  free(x);
}

static inline bigint_t*
bigint_negate
  (
    Shizu_State1* state,
    bigint_t const* x
  )
{
  if (x->sign == 0) {
    return bigint_clone(state, x);
  } else {
    bigint_t* y = malloc(sizeof(bigint_t));
    if (!y) {
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    y->cp = x->sz; // Use sz of source instead of cp of source.
    y->sz = x->sz;
    y->sign = x->sign * -1;
    y->p = malloc(sizeof(uint8_t) * y->cp);
    if (!y->p) {
      free(y);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    for (size_t i = 0; i < y->sz; ++i) {
      y->p[i] = x->p[i];
    }
    return y;
  }
}

static inline bigint_t*
bigint_clone
  (
    Shizu_State1* state,
    bigint_t const* x
  )
{
  bigint_t* y = malloc(sizeof(bigint_t));
  if (!y) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  y->cp = x->sz; // Use sz of source instead of cp of source.
  y->sz = x->sz;
  y->sign = x->sign;
  y->p = malloc(sizeof(uint8_t) * y->cp);
  if (!y->p) {
    free(y);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  for (size_t i = 0; i < y->sz; ++i) {
    y->p[i] = x->p[i];
  }
  return y;
}

static inline void
bigint_increase_cp
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t additional_cp
  )
{
  if (SIZE_MAX - x->cp < additional_cp) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  size_t new_cp = x->cp + additional_cp;
  uint8_t* new_p = realloc(x->p, sizeof(uint8_t) * new_cp);
  if (!new_p) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = new_cp;
  x->p = new_p;
}

static inline void
bigint_ensure_free_cp
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t required_free_cp
  )
{
  size_t actual_free_cp = x->cp - x->sz;
  if (actual_free_cp < required_free_cp) {
    size_t additional_free_cp = required_free_cp - actual_free_cp;
    bigint_increase_cp(state, x, additional_free_cp);
  }
}

static inline bigint_t*
bigint_from_u8
  (
    Shizu_State1* state,
    uint8_t v
  )
{
  // Conservative estimate of the maximum number of decimal digits an uint8_t value can yield.
  // UINT8_MAX is 255. Hence 3 digits is a conservative estimate.
  static const size_t DECIMAL_DIGITS = 3;
  static const uint8_t BASE = 10;

  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  if (!v) {
    x->p = malloc(sizeof(uint8_t) * 1);
    if (!x->p) {
      free(x);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    x->p[0] = 0;
    x->cp = 1;
    x->sz = 1;
    x->sign = 0;
    return x;
  }
  x->p = malloc(sizeof(uint8_t) * DECIMAL_DIGITS);
  if (!x->p) {
    free(x);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = DECIMAL_DIGITS;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  size_t i = 0;
  while (v) {
   uint8_t digit = (uint8_t)(v % BASE);
    x->p[i++] = digit;
    v /= BASE;
  }
  x->sz = i;
#else
  /*
  for (i = 0; i < x->sz / 2; ++i) {
    uint8_t t = x->p[i];
    x->p[i] = x->p[x->sz - i - 1];
    x->p[x->sz - i - 1] = t;
  }  
  */
  #error("not yet implemented")
#endif
  return x;
}

static inline bigint_t*
bigint_from_u16
  (
    Shizu_State1* state,
    uint16_t v
  )
{
  // Conservative estimate of the maximum number of decimal digits an uint16_t value can yield.
  // UINT16_MAX is 65535. Hence 5 digits is a conservative estimate.
  static const size_t DECIMAL_DIGITS = 5;
  static const uint16_t BASE = 10;

  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  if (!v) {
    x->p = malloc(sizeof(uint8_t) * 1);
    if (!x->p) {
      free(x);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    x->p[0] = 0;
    x->cp = 1;
    x->sz = 1;
    x->sign = 0;
    return x;
  }
  x->p = malloc(sizeof(uint8_t) * DECIMAL_DIGITS);
  if (!x->p) {
    free(x);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = DECIMAL_DIGITS;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  size_t i = 0;
  while (v) {
   uint8_t digit = (uint8_t)(v % BASE);
    x->p[i++] = digit;
    v /= BASE;
  }
  x->sz = i;
#else
  /*
  for (i = 0; i < x->sz / 2; ++i) {
    uint8_t t = x->p[i];
    x->p[i] = x->p[x->sz - i - 1];
    x->p[x->sz - i - 1] = t;
  }  
  */
  #error("not yet implemented")
#endif
  return x;
}

static inline bigint_t*
bigint_from_u32
  (
    Shizu_State1* state,
    uint64_t v
  )
{
  // Conservative estimate of the maximum number of decimal digits an uint32_t value can yield.
  // UINT32_MAX is 429 496 729 5. Hence 10 digits is a conservative estimate.
  static const size_t DECIMAL_DIGITS = 10;
  static const uint32_t BASE = 10;

  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  if (!v) {
    x->p = malloc(sizeof(uint8_t) * 1);
    if (!x->p) {
      free(x);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    x->p[0] = 0;
    x->cp = 1;
    x->sz = 1;
    x->sign = 0;
    return x;
  }
  x->p = malloc(sizeof(uint8_t) * DECIMAL_DIGITS);
  if (!x->p) {
    free(x);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = DECIMAL_DIGITS;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  size_t i = 0;
  while (v) {
    uint8_t digit = (uint8_t)(v % BASE);
    x->p[i++] = digit;
    v /= BASE;
  }
  x->sz = i;
#else
  /*
  for (i = 0; i < x->sz / 2; ++i) {
    uint8_t t = x->p[i];
    x->p[i] = x->p[x->sz - i - 1];
    x->p[x->sz - i - 1] = t;
  }  
  */
  #error("not yet implemented")
#endif
  return x;
}

static inline bigint_t*
bigint_from_u64
  (
    Shizu_State1* state,
    uint64_t v
  )
{
  // Conservative estimate of the maximum number of decimal digits an uint64_t value can yield.
  // UINT64_MAX is 184 467 440 737 095 516 15. Hence 20 digits is a conservative estimate.
  static const size_t DECIMAL_DIGITS = 20;
  static const uint64_t BASE = 10;

  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  if (!v) {
    x->p = malloc(sizeof(uint8_t) * 1);
    if (!x->p) {
      free(x);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    x->p[0] = 0;
    x->cp = 1;
    x->sz = 1;
    x->sign = 0;
    return x;
  }
  x->p = malloc(sizeof(uint8_t) * DECIMAL_DIGITS);
  if (!x->p) {
    free(x);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = DECIMAL_DIGITS;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  size_t i = 0;
  while (v) {
    uint8_t digit = (uint8_t)(v % BASE);
    x->p[i++] = digit;
    v /= BASE;
  }
  x->sz = i;
#else
  /*
  for (i = 0; i < x->sz / 2; ++i) {
    uint8_t t = x->p[i];
    x->p[i] = x->p[x->sz - i - 1];
    x->p[x->sz - i - 1] = t;
  }  
  */
  #error("not yet implemented")
#endif
  return x;
}

static inline bigint_t*
bigint_from_i64
  (
    Shizu_State1* state,
    int64_t v
  )
{
  // Conservative estimate of the maximum number of decimal digits an int64_t value can yield.
  // INT64_MIN is -9223372036854775808.  
  // INT64_MAX is  9223372036854775807.
  // Hence 19 digits are a conservative estimate.
  static const size_t DECIMAL_DIGITS = 19;
  static const int64_t BASE_INT64 = 10;

  bigint_t* x = malloc(sizeof(bigint_t));
  if (!x) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  if (!v) {
    x->p = malloc(sizeof(uint8_t) * 1);
    if (!x->p) {
      free(x);
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    x->p[0] = 0;
    x->cp = 1;
    x->sz = 1;
    x->sign = 0;
    return x;
  }
  x->p = malloc(sizeof(uint8_t) * DECIMAL_DIGITS);
  if (!x->p) {
    free(x);
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  x->cp = DECIMAL_DIGITS;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  size_t i = 0;
  if (v == INT64_MIN) {
    uint8_t digit = (uint8_t)((v % BASE_INT64) * INT64_C(-1));
    x->p[i++] = digit;
    v /= BASE_INT64;
  }
  if (v < 0) {
    x->sign = -1;
    v *= INT64_C(-1);
  } else {
    x->sign = +1;
  }
  while (v) {
    uint8_t digit = (uint8_t)(v % BASE_INT64);
    x->p[i++] = digit;
    v /= BASE_INT64;
  }
  x->sz = i;
#else
  /*
  for (i = 0; i < x->sz / 2; ++i) {
    uint8_t t = x->p[i];
    x->p[i] = x->p[x->sz - i - 1];
    x->p[x->sz - i - 1] = t;
  }  
  */
  #error("not yet implemented")
#endif
  return x;
}

static bool
bigint_is_negative
  (
    Shizu_State1* state,
    bigint_t const* x
  )
{ return -1 == x->sign; }

static bool
bigint_is_positive
  (
    Shizu_State1* state,
    bigint_t const* x
  )
{ return +1 == x->sign; }

static inline bool
bigint_is_zero
  (
    Shizu_State1* state,
    bigint_t const* x
  )
{ return 0 == x->sign; }

static inline bigint_t*
bigint_mul
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  )
{ 
  // The maximum length of a product of x and y is len(x) + len(y).
  size_t zcp = x->sz + y->sz;
  uint8_t* zp = malloc(sizeof(uint8_t) * zcp);
  if (!zp) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  for (size_t i = 0; i < zcp; ++i) {
    zp[i] = 0;
  }
  for (size_t i = 0; i < x->sz; ++i) {
    uint16_t v = (uint16_t)x->p[i];
    uint16_t p = UINT16_C(0);
    for (size_t j = 0; j < y->sz; ++j) {
      p += (uint16_t)(zp[i + j])
         + v * ((uint16_t)y->p[j]);
      zp[i + j] = (uint8_t)(p % 10);
      p /= 10;
    }
    zp[i + y->sz] = (uint8_t)(p % 10);
  }
  bigint_t* z = malloc(sizeof(bigint_t));
  if (!z) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  z->sz = zcp;
  z->cp = zcp;
  z->p = zp;
  if (x->sign != y->sign) {
    z->sign = -1;
  } else {
    z->sign = +1;
  }
  _bigint_trim_leading_zeroes(state, z);
  return z;
}

static inline bigint_t*
bigint_mul_pow_5_pow_2
  (
    Shizu_State1* state,
    int32_t v,
    int32_t b5,
    int32_t b2
  )
{
  bigint_t* x = NULL, * y = NULL;
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    x = bigint_from_i64(state, v);
    if (b5 < 0 || b2 < 0) {
      Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State1_jump(state);
    }
    y = bigint_pow5(state, (uint64_t)b5);
    bigint_in_situ_mul(state, x, y);
    bigint_free(state, y);
    y = NULL;

    y = bigint_pow2(state, (uint64_t)b2);
    bigint_in_situ_mul(state, x, y);
    bigint_free(state, y);
    y = NULL;

    Shizu_State1_popJumpTarget(state);
    return x;
  } else {
    Shizu_State1_popJumpTarget(state);
    if (y) {
      bigint_free(state, y);
      y = NULL;
    }
    if (x) {
      bigint_free(state, x);
      x = NULL;
    }
    Shizu_State1_jump(state);
  }
}

/// In-place divide a bigint with 10^n.
static inline void
bigint_in_situ_div_10
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t n
  )
{
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  if (x->sz > 0) {
    x->sz -= n;
  } else {
    x->sz = 0;
  }
#else
  #error("not yet implemented")
#endif
}

static inline void
bigint_in_situ_mul
  (
    Shizu_State1* state,
    bigint_t* x,
    bigint_t const* y
  )
{
  bigint_t* temporary = bigint_mul(state, x, y);
  _bigint_swap(state, x, temporary);
  bigint_free(state, temporary);
}

/// In-place multiply a bigint with 10^n.
static inline void
bigint_in_situ_mul_10
  (
    Shizu_State1* state,
    bigint_t* x,
    size_t n
  )
{
  bigint_ensure_free_cp(state, x, n);
  size_t old_sz = x->sz;
  // @todo Check for overflow.
  size_t new_sz = x->sz + n;
#if BIGINT_CFG_ORDER == BIGINT_CFG_ORDER_LSD_TO_MSD
  for (size_t i = old_sz; i < new_sz; ++i) {
    x->p[i] = 0;
  }
#else
  #error("not yet implemented")  
#endif
  x->sz = new_sz; 
}

// In-place multiply a bigint with 2^n.
static inline void
bigint_in_situ_mul_2
  (
    Shizu_State1* state,
    bigint_t* x,
    uint64_t n
  )
{
  bigint_t* y = bigint_pow2(state, n);
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    bigint_in_situ_mul(state, x, y);
    Shizu_State1_popJumpTarget(state);
    bigint_free(state, y);
  } else {
    Shizu_State1_popJumpTarget(state);
    bigint_free(state, y);
    Shizu_State1_jump(state);
  }
}

static bigint_t*
bigint_pow2
  (
    Shizu_State1* state,
    uint64_t n
  )
{
  if (n < 32) {
    return bigint_from_u32(state, (uint32_t)(UINT32_C(1) << n));
  } else if (n < 63) {
    return bigint_from_u64(state, UINT64_C(1) << n);
  } else {
    // We would like to use an equivalence 2^n = 2^(n/2 + n/2) = 2^(n/2) * 2^(n/2) and compute 2^(n/2) recursively.
    // However, n/2 is not an integer if n is odd. So we use a different equivalence.
    // Let p := floor(n/2) and q := n - p and compute p and q.
    // This gives 2^p * 2^q = 2^(p + q) = 2^(floor(n/2) + (n - p)) = 2^(floor(n/2) + n - floor(n/2)) = 2^n.
    uint64_t p = n >> 1;
    uint64_t q = n - p;
    bigint_t* x = NULL, * y = NULL, * z = NULL;
    Shizu_JumpTarget jumpTarget;
    Shizu_State1_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      x = bigint_pow2(state, p);
      y = bigint_pow2(state, q);
      z = bigint_mul(state, x, y);
      Shizu_State1_popJumpTarget(state);
      bigint_free(state, y);
      y = NULL;
      bigint_free(state, x);
      x = NULL;
      return z;
    } else {
      Shizu_State1_popJumpTarget(state);
      if (z) {
        bigint_free(state, z);
        z = NULL;
      }
      if (y) {
        bigint_free(state, y);
        y = NULL;
      }
      if (x) {
        bigint_free(state, x);
        x = NULL;
      }
      Shizu_State1_jump(state);
    }
  }
}

static bigint_t*
bigint_pow5
  (
    Shizu_State1* state,
    uint64_t n
  )
{
#define POW5 UINT64_C(5)*UINT64_C(5)*UINT64_C(5)*UINT64_C(5)*UINT64_C(5)
  // Table with powers of 5 from 0 to 27.
  // 5^27 is the greatest power of 5 less than or equal to UINT64_MAX.
  // UINT64_MAX is 18,446,744,073,709,551,615.
  // 5^27 is  7,450,580,596,923,828,125.
  static const uint64_t powersOf5[] = {
    UINT64_C(1),// 5^0
    UINT64_C(5),// 5^1
    UINT64_C(5) * UINT64_C(5),// 5^2
    UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^3
    UINT64_C(5) * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^4
    POW5,// 5^5
    POW5 * UINT64_C(5),// 5^6
    POW5 * UINT64_C(5) * UINT64_C(5),// 5^7
    POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^8
    POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^9
    POW5 * POW5,// 5^10
    POW5 * POW5 * UINT64_C(5),// 5^11
    POW5 * POW5 * UINT64_C(5) * UINT64_C(5),// 5^12
    POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^13
    POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^14
    POW5 * POW5 * POW5,// 5^15
    POW5 * POW5 * POW5 * UINT64_C(5),// 5^16
    POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5),// 5^17
    POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^18
    POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^19
    POW5 * POW5 * POW5 * POW5,// 5^20
    POW5 * POW5 * POW5 * POW5 * UINT64_C(5),// 5^21
    POW5 * POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5),// 5^22
    POW5 * POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^23
    POW5 * POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5) * UINT64_C(5) * UINT64_C(5),// 5^24
    POW5 * POW5 * POW5 * POW5 * POW5,// 5^25
    POW5 * POW5 * POW5 * POW5 * POW5 * UINT64_C(5),// 5^26
    POW5 * POW5 * POW5 * POW5 * POW5 * UINT64_C(5) * UINT64_C(5),// 5^27
  };
  static const powersOf5Size = sizeof(powersOf5) / sizeof(uint64_t);
  static_assert(sizeof(powersOf5) / sizeof(uint64_t) == 28, "<internal error>");
  if (n < powersOf5Size) {
    return bigint_from_u64(state, powersOf5[n]);
  } else {
    // We would like to use an equivalence 5^n = 5^(n/2 + n/2) = 5^(n/2) * 5^(n/2) and compute 5^(n/2) recursively.
    // However, n/2 is not an integer if n is odd. So we use a different equivalence.
    // Let p := floor(n/2) and q := n - p and compute p and q.
    // This gives 5^p * 5^q = 5^(p + q) = 5^(floor(n/2) + (n - p)) = 5^(floor(n/2) + n - floor(n/2)) = 5^n.
    uint64_t p = n >> 1;
    uint64_t q = n - p;
    bigint_t* x = NULL, * y = NULL, * z = NULL;
    Shizu_JumpTarget jumpTarget;
    Shizu_State1_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      x = bigint_pow5(state, p);
      y = bigint_pow5(state, q);
      z = bigint_mul(state, x, y);
      Shizu_State1_popJumpTarget(state);
      bigint_free(state, y);
      y = NULL;
      bigint_free(state, x);
      x = NULL;
      return z;
    } else {
      Shizu_State1_popJumpTarget(state);
      if (z) {
        bigint_free(state, z);
        z = NULL;
      }
      if (y) {
        bigint_free(state, y);
        y = NULL;
      }
      if (x) {
        bigint_free(state, x);
        x = NULL;
      }
      Shizu_State1_jump(state);
    }
  }
}

static inline bigint_t*
bigint_subtract
  (
    Shizu_State1* state,
    bigint_t const* x,
    bigint_t const* y
  )
{
  bigint_t* c = NULL;
  bigint_t* b = bigint_negate(state, y);
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    c = bigint_add(state, x, y);
    Shizu_State1_popJumpTarget(state);
    bigint_free(state, b);
    return c;
  } else {
    Shizu_State1_popJumpTarget(state);
    bigint_free(state, b);
    Shizu_State1_jump(state);
  }
}

static void
_bigint_trim_leading_zeroes
  (
    Shizu_State1* state,
    bigint_t* x
  )
{
  size_t count = 0;
  for (size_t i = x->sz; i > 0; --i) {
    if (x->p[i - 1]) {
      break;
    }
    count++;
  }
  if (count == x->sz) {
    // Retain one digit.
    count--;
  }
  x->sz -= count;
  if (x->sz == 1 && x->p[0] == 0) {
    x->sign = 0;
  }
}

// no error
static void
_bigint_swap
  (
    Shizu_State1* state,
    bigint_t* x,
    bigint_t* y
  )
{
#define swap(Type,X,Y) \
  { \
    Type t = X; \
    X = Y; \
    Y = t; \
  }
  if (x == y) {
    return;
  } else {
    swap(size_t, x->sz, y->sz);
    swap(size_t, x->cp, y->cp);
    swap(uint8_t*, x->p, y->p);
    swap(int8_t, x->sign, y->sign);
  }
#undef swap
}

#if defined(Shizu_Configuration_WithTests)

// Sanity check certain constants.
void
bigint_tests
  (
    Shizu_State1* state
  );

#endif

#endif SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_BIGINT_H_INCLUDED
 