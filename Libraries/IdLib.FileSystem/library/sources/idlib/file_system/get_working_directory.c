/*
  IdLib File System
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

#include "idlib/file_system/get_working_directory.h"

#include "idlib/process.h"

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // get_current_dir_name
  #include <unistd.h>

  // strlen
  #include <string.h>

#else

  #error("operating system not (yet) supported")

#endif

// SIZE_MAX
#include <limits.h>

// malloc, free
#include <malloc.h>

int
idlib_get_working_directory
  (
    void* context,
    idlib_get_working_directory_callback* callback
  )
{
  if (!callback) {
    return IDLIB_ARGUMENT_INVALID;
  }
#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // n = number of characters (required) including the zero terminator.
  DWORD n = (DWORD)GetCurrentDirectory(0, NULL);
  if (!n || n > SIZE_MAX) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  char* p = malloc((size_t)n);
  if (!p) {
    return IDLIB_ALLOCATION_FAILED;
  }
  // m = number of characters (written) excluding the zero terminator.
  DWORD m = GetCurrentDirectory(n, p);
  if (!m || m != n - 1) {
    free(p);
    p = NULL;
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (!(*callback)(context, p, m)) {
    free(p);
    p = NULL;
    return IDLIB_ABORTED;
  }
  free(p);
  p = NULL;

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  char* p = get_current_dir_name();
  if (!p) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (!(*callback)(context, p, strlen(p))) {
    free(p);
    p = NULL;
    return IDLIB_ABORTED;
  }
  free(p);
  p = NULL;

#else

  #error("operating system not (yet) supported")

#endif  
  return IDLIB_SUCCESS;
}