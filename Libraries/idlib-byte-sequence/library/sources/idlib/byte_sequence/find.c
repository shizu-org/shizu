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

#include "idlib/byte_sequence/find.h"

#include <assert.h>

// memchr
#include <string.h>

idlib_status idlib_byte_sequence_naive_find(void const* p, size_t n, void const* q, size_t m, bool* found, size_t* index) {
  /* Abstract memory is considered to be an array of 'unsigned char' values, not an array of 'char' values.  See ISO C 99 section 6.2.6.1.  */
  if (!p || !q || !found || !index) {
    return IDLIB_ARGUMENT_INVALID;
  }
  if (0 == m) {
    *index = 0;
    *found = true;
    return IDLIB_SUCCESS;
  }
  if (n < m) {
    *index = n;
    *found = false;
    return IDLIB_SUCCESS;
  }

  char const* haystack = p;
  char const* current_haystack = p;
  size_t haystack_length = n;
  size_t current_haystack_length = n;
  char const* needle = q;
  size_t needle_length = m;
  do {
    char const* temporary = memchr(current_haystack, *needle, current_haystack_length);
    if (!temporary) {
      *index = temporary - haystack;
      *found = false;
      return IDLIB_SUCCESS;
    }
    if (!memcmp(temporary, needle, needle_length)) {
      *index = temporary - haystack;
      *found = true;
      return IDLIB_SUCCESS;
    }
    current_haystack++;
    current_haystack_length--;
  } while (haystack_length > needle_length);
  *index = n;
  *found = false;
  return IDLIB_SUCCESS;
}


