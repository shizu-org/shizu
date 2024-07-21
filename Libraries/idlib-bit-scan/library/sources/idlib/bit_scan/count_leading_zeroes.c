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

#include "idlib/bit_scan/count_leading_zeroes.h"

size_t
idlib_count_leading_zeroes_u8
	(
		uint8_t x
	)
{
	for (size_t i = 8; i > 0; --i) {
		uint8_t mask = 1 << (i - 1);
		if (x & mask) {
			return 8 - i;
		}
	}
	return 8;
}

size_t
idlib_count_leading_zeroes_u16
	(
		uint16_t x
	)
{
	for (size_t i = 16; i > 0; --i) {
		uint16_t mask = 1 << (i - 1);
		if (x & mask) {
			return 16 - i;
		}
	}
	return 16;
}

size_t
idlib_count_leading_zeroes_u32
	(
		uint32_t x
	)
{
	for (size_t i = 32; i > 0; --i) {
		uint32_t mask = 1 << (i - 1);
		if (x & mask) {
			return 32 - i;
		}
	}
	return 32;
}

size_t
idlib_count_leading_zeroes_u64
	(
		uint64_t x
	)
{
	for (size_t i = 64; i > 0; --i) {
		uint64_t mask = ((uint64_t)1) << (i - 1);
		if (x & mask) {
			return 64 - i;
		}
	}
	return 64;
}

size_t
idlib_count_leading_zeroes_sz
	(
		size_t x
	)
{
	#if Shizu_Configuration_InstructionSetArchitecture_X64 == Shizu_Configuration_InstructionSetArchitecture
		return idlib_count_leading_zeroes_u64((uint64_t)x);
	#elif Shizu_Configuration_InstructionSetArchitecture_X32 == Shizu_Configuration_InstructionSetArchitecture
		return idlib_count_leading_zeroes_u32((uint32_t)x);
	#else
		#error("operating system not (yet) supported")
	#endif
}
