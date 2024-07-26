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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/floatBits.h"

typedef union f32 {
  uint32_t u32;
  float f32;
} f32;

typedef union f64 {
  uint64_t u64;
  double f64;
} f64;

/// @brief Get the bits of an IEEE754 32 bit floating point value as an uint32_t value. 
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The IEEE754 32 bit floating point value.
/// @return The uint32_t value.
uint32_t
Shizu_f32ToU32
  (
    Shizu_State1* state,
    float x
  )
{
  f32 helper = { .f32 = x };
  return helper.u32;
}
  
/// @brief Get the bits of an uint32_t value as an IEEE754 32 bit floating point value.
/// @param state A pointer to the Shizu service level 1 state.
/// @param x The uint32_t value.
/// @return The IEEE754 32 bit floating point value.
float
Shizu_s32ToF32
  (
    Shizu_State1* state,
    uint32_t x
  )
{
  f32 helper = { .u32 = x };
  return helper.f32;
}

uint64_t
Shizu_f64ToU64
  (
    Shizu_State1* state,
    double x
  )
{
  f64 helper = { .f64 = x };
  return helper.u64;
}

double
Shizu_u64ToF64
  (
    Shizu_State1* state,
    uint64_t x
  )
{
  f64 helper = { .u64 = x };
  return helper.f64;
}
