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

#include "idlib/byte_sequence.h"

// memcpy, memmove, memset, memcmp
#include <string.h>

#include "idlib/byte_sequence/find.h"

idlib_status idlib_byte_sequence_copy(void* p, void const* q, size_t n) {
  if (!p || !q || n > SIZE_MAX - 2) {
    return IDLIB_ARGUMENT_INVALID;
  }
  memcpy(p, q, n);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_move(void* p, void const* q, size_t n) {
  if (!p || !q  || n > SIZE_MAX - 2) {
    return IDLIB_ARGUMENT_INVALID;
  }
  memmove(p, q, n);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_fill_zero(void* p, size_t n) {
  if (!p || n > SIZE_MAX - 2) {
    return IDLIB_ARGUMENT_INVALID;
  }
  memset(p, 0, n);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_fill_value(void* p, size_t n, uint8_t v) {
  if (!p) {
    return IDLIB_ARGUMENT_INVALID;
  }
  memset(p, v, n);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_compare(int8_t* RETURN, void const* p, void const* q, size_t n) {
  if (!RETURN || !p || n > SIZE_MAX - 2 || !q) {
    return IDLIB_ARGUMENT_INVALID;
  }
  int temporary = memcmp(p, q, n);
  *RETURN = temporary < 0 ? -1 : (temporary > 0 ? +1 : 0);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_find_value(void const* p, size_t n, uint8_t v, bool* found, size_t* index) {
  if (!p || !found || !index) {
    return IDLIB_ARGUMENT_INVALID;
  }
  void const* q = memchr(p, v, n);
  if (!q) {
    *found = false;
    *index = n;
  } else {
    *found = true;
    *index = ((uint8_t const*)q) - ((uint8_t const*)p);
  }
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_find(void const* p, size_t n, void const* q, size_t m, bool* found, size_t* index) {
  return idlib_byte_sequence_naive_find(p, n, q, m, found, index);
}

idlib_status idlib_byte_sequence_starts_with(bool* RETURN, void const* p, size_t n, void const* q, size_t m) {
  if (!RETURN || !p || n > SIZE_MAX - 2 || !q || m > SIZE_MAX - 2) {
    return IDLIB_ARGUMENT_INVALID;
  }
  if (0 == m) {
    *RETURN = true;
    return IDLIB_SUCCESS;
  }
  // If haystack is shorter than needle, then the former cannot start with the latter.
  if (n < m) {
    *RETURN = false;
    return IDLIB_SUCCESS;
  }
  *RETURN = !memcmp(p, q, m);
  return IDLIB_SUCCESS;
}

idlib_status idlib_byte_sequence_ends_with(bool* RETURN, void const* p, size_t n, void const* q, size_t m) {
  if (!RETURN || !p || n > SIZE_MAX - 2 || !q || m > SIZE_MAX - 2) {
    return IDLIB_ARGUMENT_INVALID;
  }
  if (0 == m) {
    *RETURN = true;
    return IDLIB_SUCCESS;
  }
  // If haystack is shorter than needle, then the former cannot end with the latter.
  if (n < m) {
    *RETURN = false;
    return IDLIB_SUCCESS;
  }
  *RETURN = !memcmp(((char const*)p) + n - m, q, m);
  return IDLIB_SUCCESS;
}

