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

#include "idlib/file_system/get_file_type.h"

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

// errno, ENOTDIR, ENAMETOOLONG, EACCES, ELOOP, ENOMEN, EFAULT, EBADF
#include <errno.h>

//#include <sys/types.h>

// stat (function), stat (struct)
#include <sys/stat.h>

//#include <unistd.h>

#endif

int
idlib_get_file_type
  (
    char const *path_name,
    idlib_file_type* file_type
  )
{
  if (!path_name || !file_type) {
    return IDLIB_ARGUMENT_INVALID;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  DWORD attributes = GetFileAttributesA(path_name);
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    DWORD error = GetLastError();
    SetLastError(0);
    if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) {
      return IDLIB_FILE_NOT_FOUND;
    } else if (error == ERROR_INVALID_NAME ||
               error == ERROR_INVALID_DRIVE ||
               error == ERROR_NOT_READY ||
               error == ERROR_INVALID_PARAMETER ||
               error == ERROR_BAD_PATHNAME ||
               error == ERROR_BAD_NETPATH) {
      return IDLIB_ARGUMENT_INVALID;
    } else {
      // The case was not properly handled.
      return IDLIB_ENVIRONMENT_FAILED;
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & attributes)) {
      *file_type = idlib_file_type_directory;
    } else {
      *file_type = idlib_file_type_regular;
    }
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  struct stat t;
  int result = stat(path_name, &t);
  if (0 == result) {
    if (0 != S_ISDIR(t.st_mode))
      *file_type = idlib_file_type_directory;
    else if (0 != S_ISREG(t.st_mode))
      *file_type = idlib_file_type_regular;
    else
      *file_type = idlib_file_type_unknown;
  } else {
    if (errno == ENOTDIR || errno == ENOENT || errno == ENAMETOOLONG || errno == EACCES)
      // ENOTDIR, ENOENT, ENAMETOOLONG, and EACCES all imply that the file does not exist or is not accessible due to permission constraints.
      return IDLIB_FILE_NOT_FOUND;
    else if (errno == EOVERFLOW) // EOVERFLOW indicates that some file exists but is not accessible due to technical constraints.
      return IDLIB_FILE_NOT_FOUND;
    else /*if (errno == ELOOP || errno == ENOMEN || errno == EFAULT || errno == EBADF)*/
      return IDLIB_ENVIRONMENT_FAILED;
  }

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}
