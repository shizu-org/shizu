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
#include "Shizu/Runtime/powerOfTwoGreaterThan.h"

#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Status.h"
#include "Shizu/Runtime/countLeadingZeroes.h"

uint8_t
Shizu_powerOfTwoGreaterThanU8
  (
    Shizu_State1* state,
    uint8_t x
  )
{
  size_t k = Shizu_countLeadingZeroesU8(state, x);
  if (0 == k) {
    Shizu_State1_setStatus(state, Shizu_Status_NotRepresentable);
    Shizu_State1_jump(state);
  }
  return UINT8_C(1) << (8 - k);
}

uint16_t
Shizu_powerOfTwoGreaterThanU16
  (
    Shizu_State1* state,
    uint16_t x
  )
{
  size_t k = Shizu_countLeadingZeroesU16(state, x);
  if (0 == k) {
    Shizu_State1_setStatus(state, Shizu_Status_NotRepresentable);
    Shizu_State1_jump(state);
  }
  return UINT16_C(1) << (16 - k);
}

uint32_t
Shizu_powerOfTwoGreaterThanU32
  (
    Shizu_State1* state,
    uint32_t x
  )
{
  size_t k = Shizu_countLeadingZeroesU32(state, x);
  if (0 == k) {
    Shizu_State1_setStatus(state, Shizu_Status_NotRepresentable);
    Shizu_State1_jump(state);
  }
  return UINT32_C(1) << (32 - k);
}

uint64_t
Shizu_powerOfTwoGreaterThanU64
  (
    Shizu_State1* state,
    uint64_t x
  )
{
  size_t k = Shizu_countLeadingZeroesU64(state, x);
  if (0 == k) {
    Shizu_State1_setStatus(state, Shizu_Status_NotRepresentable);
    Shizu_State1_jump(state);
  }
  return UINT64_C(1) << (64 - k);
}

size_t
Shizu_powerOfTwoGreaterThanSz
  (
    Shizu_State1* state,
    size_t x
  )
{
#if Shizu_Configuration_InstructionSetArchitecture_X64 == Shizu_Configuration_InstructionSetArchitecture
  return Shizu_powerOfTwoGreaterThanU64(state, (uint64_t)x);
#elif Shizu_Configuration_InstructionSetArchitecture_X86 == Shizu_Configuration_InstructionSetArchitecture
  return Shizu_powerOfTwoGreaterThanU32(state, (uint32_t)x);
#else
  #error("instruction set architecture not (yet) supported")
#endif 
}
