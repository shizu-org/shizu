/*
  IdLib Byte Sequence
  Copyright (C) 2018-2024 Michael Heilmann. All rights reserved.

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

#if !defined(IDLIB_BYTE_SEQUENCE_H_INCLUDED)
#define IDLIB_BYTE_SEQUENCE_H_INCLUDED

#include "idlib/process.h"

#include <stdbool.h>

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_copy.md
idlib_status idlib_byte_sequence_copy(void* p, void const* q, size_t n);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_move.md
idlib_status idlib_byte_sequence_move(void* p, void const* q, size_t n);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_fill_zero.md
idlib_status idlib_byte_sequence_fill_zero(void* p, size_t n);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_fill_value.md
idlib_status idlib_byte_sequence_fill_value(void* p, size_t n, uint8_t v);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_compare.md
idlib_status idlib_byte_sequence_compare(int8_t* RETURN, void const* p, void const* q, size_t n);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_find_value.md
idlib_status idlib_byte_sequence_find_value(void const* p, size_t n, uint8_t v, bool* found, size_t* index);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_find.md
idlib_status idlib_byte_sequence_find(void const* p, size_t n, void const *q, size_t m, bool* found, size_t* index);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_starts_with.md
idlib_status idlib_byte_sequence_starts_with(bool* RETURN, void const* p, size_t n, void const* q, size_t m);

// https://github.com/michaelheilmann/idlib-byte-sequence/blob/main/documentation/idlib_byte_sequence_ends_with.md
idlib_status idlib_byte_sequence_ends_with(bool* RETURN, void const* p, size_t n, void const* q, size_t m);

#endif // IDLIB_BYTE_SEQUENCE_H_INCLUDED
