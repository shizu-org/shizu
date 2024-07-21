/*
  IdLib Byte Sequence
  Copyright (C) 2023-2024 Michael Heilmann. All rights reserved.

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

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

static idlib_status
test1
  (
  )
{
  bool result;
  char const* p = "x";
  size_t n = 1;
  uint8_t const v = 'y';
  bool found = false;
  size_t index = 0;
  if (idlib_byte_sequence_find_value(p, n, v, &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must not be NULL.
  if (!idlib_byte_sequence_find_value(p, n, v, &found, NULL)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must not be NULL.
  if (!idlib_byte_sequence_find_value(p, n, v, NULL, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must not be NULL.
  if (!idlib_byte_sequence_find_value(NULL, n, v, &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  return IDLIB_SUCCESS;
}

static idlib_status
test2
  (
  )
{
  bool result;
  char const* p = "xyzxyz";
  size_t n = 6;
  uint8_t const v = 'y';
  bool found = false;
  size_t index = 0;
  
  // Must be found at 0.
  if (idlib_byte_sequence_find_value(p, n, 'x', &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (!found || 0 != index) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must be found at 1.
  if (idlib_byte_sequence_find_value(p, n, 'y', &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (!found || 1 != index) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must be found at 2.
  if (idlib_byte_sequence_find_value(p, n, 'z', &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (!found || 2 != index) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Must not be found at 6.
  if (idlib_byte_sequence_find_value(p, n, 'w', &found, &index)) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (found || 6 != index) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  return IDLIB_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (test1()) {
    return EXIT_FAILURE;
  }
  if (test2()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

