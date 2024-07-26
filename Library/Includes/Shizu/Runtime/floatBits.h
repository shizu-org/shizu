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

#if !defined(SHIZU_RUNTIME_FLOATBITS_H_INCLUDED)
#define SHIZU_RUNTIME_FLOATBITS_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/floatBits.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif

#include "Shizu/Runtime/Configure.h"
#include <stdbool.h>
#include <inttypes.h>
typedef struct Shizu_State1 Shizu_State1;

/// @brief Get the bits of an IEEE754 32 bit floating point value as an uint32_t value. 
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The IEEE754 32 bit floating point value.
/// @return The uint32_t value.
uint32_t
Shizu_f32ToU32
  (
    Shizu_State1* state,
    float x
  );
  
/// @brief Get the bits of an uint32_t value as an IEEE754 32 bit floating point value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The uint32_t value.
/// @return The IEEE754 32 bit floating point value.
float
Shizu_s32ToF32
  (
    Shizu_State1* state,
    uint32_t x
  );

/// @brief Get the bits of an IEEE754 64 bit floating point value as an uint64_t value. 
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The IEEE754 64 bit floating point value.
/// @return The uint64_t value.
uint64_t
Shizu_f64ToU64
  (
    Shizu_State1* state,
    double x
  );

/// @brief Get the bits of an uint64_t value as an IEEE754 64 bit floating point value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The uint64_t value.
/// @return The IEEE754 64 bit floating point value.
double
Shizu_u64ToF64
  (
    Shizu_State1* state,
    uint64_t x
  );

#endif // SHIZU_RUNTIME_FLOATTOBITS_H_INCLUDED
