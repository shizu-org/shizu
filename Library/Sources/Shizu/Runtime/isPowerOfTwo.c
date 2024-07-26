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
#include "Shizu/Runtime/isPowerOfTwo.h"

bool
Shizu_isPowerOfTwoU8
  (
    Shizu_State1* state,
    uint8_t x
  )
{
  if (!x) return false;
  else return 0 == (x & (x - 1));
}

bool
Shizu_isPowerOfTwoU16
  (
    Shizu_State1* state,
    uint16_t x
  )
{
  if (!x) return false;
  else return 0 == (x & (x - 1));
}

bool
Shizu_isPowerOfTwoU32
  (
    Shizu_State1* state,
    uint32_t x
  )
{
  if (!x) return false;
  else return 0 == (x & (x - 1));
}

bool
Shizu_isPowerOfTwoU64
  (
    Shizu_State1* state,
    uint64_t x
  )
{
  if (!x) return false;
  else return 0 == (x & (x - 1));
}

bool
Shizu_isPowerOfTwoSz
  (
    Shizu_State1* state,
    size_t x
  )
{
#if Shizu_Configuration_InstructionSetArchitecture_X64 == Shizu_Configuration_InstructionSetArchitecture
  static_assert(SIZE_MAX <= UINT64_MAX, "assertion `SIZE_MAX <= UINT64_MAX` failed");
  return Shizu_isPowerOfTwoU64(state, (uint64_t)x);
#elif Shizu_Configuration_InstructionSetArchitecture_X86 == Shizu_Configuration_InstructionSetArchitecture
  static_assert(SIZE_MAX <= UINT32_MAX, "assertion `SIZE_MAX <= UINT32_MAX` failed");
  return Shizu_isPowerOfTwoU32(state, (uint32_t)x);
#else
  #error("instruction set architecture not (yet) supported")
#endif
}
