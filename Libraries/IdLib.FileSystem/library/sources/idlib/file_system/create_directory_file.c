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

#include "idlib/file_system/create_directory_file.h"

#include "idlib/file_system/configure.h"
#include "idlib/file_system/status.h"

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // For errno.
  #include <errno.h>

  // For mkdir.
  #include <sys/stat.h>

#else

  #error("operating system not (yet) supported")

#endif

int
idlib_create_directory_file
  (
    char const* path_name
  )
{

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  BOOL result = CreateDirectory(path_name, NULL);
  if (!result) {
    return IDLIB_ENVIRONMENT_FAILED;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  int result = mkdir(path_name, 0755);
  if (-1 == result) {
    return IDLIB_ENVIRONMENT_FAILED;
  }

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}
