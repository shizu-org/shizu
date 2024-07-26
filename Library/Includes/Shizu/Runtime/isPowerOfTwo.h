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

#if !defined(SHIZU_RUNTIME_ISPOWEROFTWO_H_INCLUDED)
#define SHIZU_RUNTIME_ISPOWEROFTWO_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/isPowerOfTwo.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif

#include "Shizu/Runtime/Configure.h"
#include <stdbool.h>
#include <inttypes.h>
typedef struct Shizu_State1 Shizu_State1;

/// @brief Get if a value is a power of two (that is, if it is 1, 2, 4, ...).
/// @param state A pointer to the Shizu Service Level 1 state.
/// @param x The value.
/// @return @a true if the value is a power of two. @a false if the value is not a power of two.
bool
Shizu_isPowerOfTwoU8
  (
    Shizu_State1* state,
    uint8_t x
  );

/// @brief Get if a value is a power of two (that is, if it is 1, 2, 4, ...).
/// @param state A pointer to the Shizu Service Level 1 state.
/// @param x The value.
/// @return @a true if the value is a power of two. @a false if the value is not a power of two.
bool
Shizu_isPowerOfTwoU16
  (
    Shizu_State1* state,
    uint16_t x
  );

/// @brief Get if a value is a power of two (that is, if it is 1, 2, 4, ...).
/// @param state A pointer to the Shizu Service Level 1 state.
/// @param x The value.
/// @return @a true if the value is a power of two. @a false if the value is not a power of two.
bool
Shizu_isPowerOfTwoU32
  (
    Shizu_State1* state,
    uint32_t x
  );

/// @brief Get if a value is a power of two (that is, if it is 1, 2, 4, ...).
/// @param state A pointer to the Shizu Service Level 1 state.
/// @param x The value.
/// @return @a true if the value is a power of two. @a false if the value is not a power of two.
bool
Shizu_isPowerOfTwoU64
  (
    Shizu_State1* state,
    uint64_t x
  );

/// @brief Get if a value is a power of two (that is, if it is 1, 2, 4, ...).
/// @param state A pointer to the Shizu Service Level 1 state.
/// @param x The value.
/// @return @a true if the value is a power of two. @a false if the value is not a power of two.
bool
Shizu_isPowerOfTwoSz
  (
    Shizu_State1* state,
    size_t x
  );

#endif // SHIZU_RUNTIME_ISPOWEROFTWO_H_INCLUDED
