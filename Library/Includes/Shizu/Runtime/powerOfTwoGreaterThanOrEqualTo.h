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

#if !defined(SHIZU_RUNTIME_POWEROFTWOGREATERTHANOREQUALTO_H_INCLUDED)
#define SHIZU_RUNTIME_POWEROFTWOGREATERTHANOREQUALTO_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
#error("Do not include `Shizu/Runtime/powerOfTwoGreaterThanOrEqualTo.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif

#include "Shizu/Runtime/Configure.h"
#include <stdbool.h>
#include <inttypes.h>
typedef struct Shizu_State1 Shizu_State1;

/// @brief Get the least power of two greater than or equal to a value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The value.
/// @return The least power of two greater than or equal to the value.
/// @error Shizu_Status_NotRepresentable if that power of two can be represented by the value type.
uint8_t
Shizu_powerOfTwoGreaterThanOrEqualToU8
  (
    Shizu_State1* state,
    uint8_t x
  );

/// @brief Get the least power of two greater than or equal to a value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The value.
/// @return The least power of two greater than or equal to the value.
/// @error Shizu_Status_NotRepresentable if that power of two can be represented by the value type.
uint16_t
Shizu_powerOfTwoGreaterThanOrEqualToU16
  (
    Shizu_State1* state,
    uint16_t x
  );

/// @brief Get the least power of two greater than or equal to a value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The value.
/// @return The least power of two greater than or equal to the value.
/// @error Shizu_Status_NotRepresentable if that power of two can be represented by the value type.
uint32_t
Shizu_powerOfTwoGreaterThanOrEqualToU32
  (
    Shizu_State1* state,
    uint32_t x
  );

/// @brief Get the least power of two greater than or equal to a value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The value.
/// @return The least power of two greater than or equal to the value.
/// @error Shizu_Status_NotRepresentable if that power of two can be represented by the value type.
uint64_t
Shizu_powerOfTwoGreaterThanOrEqualToU64
  (
    Shizu_State1* state,
    uint64_t x
  );

/// @brief Get the least power of two greater than or equal to a value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The value.
/// @return The least power of two greater than or equal to the value.
/// @error Shizu_Status_NotRepresentable if that power of two can be represented by the value type.
size_t
Shizu_powerOfTwoGreaterThanOrEqualToSz
  (
    Shizu_State1* state,
    size_t x
  );

#endif // SHIZU_RUNTIME_POWEROFTWOGREATERTHANOREQUALTO_H_INCLUDED