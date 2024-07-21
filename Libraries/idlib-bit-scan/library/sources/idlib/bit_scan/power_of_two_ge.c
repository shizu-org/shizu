/*
  IdLib Bit Scan
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

#include "idlib/bit_scan/power_of_two_ge.h"

#include "idlib/bit_scan.h"

int
idlib_power_of_two_ge_u8
  (
    uint8_t* RETURN,
    uint8_t x
  )
{ 
  if (idlib_is_power_of_two_u8(x)) {
    *RETURN = x;
    return IDLIB_SUCCESS;
  } else {
    return idlib_power_of_two_gt_u8(RETURN, x);
  }
}

int
idlib_power_of_two_ge_u16
  (
    uint16_t* RETURN,
    uint16_t x
  )
{ 
  if (idlib_is_power_of_two_u16(x)) {
    *RETURN = x;
    return IDLIB_SUCCESS;
  } else {
    return idlib_power_of_two_gt_u16(RETURN, x);
  }
}

int
idlib_power_of_two_ge_u32
  (
    uint32_t* RETURN,
    uint32_t x
  )
{ 
  if (idlib_is_power_of_two_u32(x)) {
    *RETURN = x;
    return IDLIB_SUCCESS;
  } else {
    return idlib_power_of_two_gt_u32(RETURN, x);
  }
}

int
idlib_power_of_two_ge_u64
  (
    uint64_t* RETURN,
    uint64_t x
  )
{ 
  if (idlib_is_power_of_two_u64(x)) {
    *RETURN = x;
    return IDLIB_SUCCESS;
  } else {
    return idlib_power_of_two_gt_u64(RETURN, x);
  }
}

int
idlib_power_of_two_ge_sz
  (
    size_t* RETURN,
    size_t x
  )
{
#if IDLIB_INSTRUCTION_SET_ARCHITECTURE_X64 == IDLIB_INSTRUCTION_SET_ARCHITECTURE
  return idlib_power_of_two_ge_u64(RETURN, x);
#elif IDLIB_INSTRUCTION_SET_ARCHITECTURE_X86 == IDLIB_INSTRUCTION_SET_ARCHITECTURE
  return idlib_power_of_two_ge_u32(RETURN, x);
#else
  #error("instruction set architecture not (yet) supported")
#endif 
}
