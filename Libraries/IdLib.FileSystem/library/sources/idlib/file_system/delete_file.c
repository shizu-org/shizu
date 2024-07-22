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

#include "idlib/file_system/delete_file.h"

#include "idlib/file_system/configure.h"
#include "idlib/file_system/status.h"
#include "idlib/file_system/get_file_type.h"

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // For errno.
  #include <errno.h>

  // For remove.
  #include <stdio.h>

  // For strcmp.
  #include <string.h>

#else

  #error("operating system not (yet) supported")

#endif

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

/**
 * @internal
 * @brief Delete a directory file.
 * @param path_name The pathname to the directory file.
 * @return #IDLIB_SUCCESS on success. A non-zero value on failure.
 * In particular, this function returns #IDLIB_FS_FILE_NOT_FOUND if the directory file was not found.
 * This function fails if the directory is not empty.
 */
static int
idlib_delete_directory_file
  (
    char const* path_name
  );

/**
 * @internal
 * @brief Delete a regular file.
 * @param pathname The pathname to the directory file.
 * @return #IDLIB_SUCCESS on success. A non-zero value on failure.
 */
static int
idlib_delete_regular_file
  (
    char const* path_name
  );

static int
idlib_delete_directory_file
  (
    char const* pathname
  )
{
  BOOL result = RemoveDirectory(pathname);
  if (!result) {
    DWORD error = GetLastError();
    if (ERROR_DIR_NOT_EMPTY == error) {
      return IDLIB_DIRECTORY_NOT_EMPTY;
    } else if (ERROR_ACCESS_DENIED == error || ERROR_FILE_NOT_FOUND == error) {
      return IDLIB_FILE_NOT_FOUND;
    } else {
      return IDLIB_ENVIRONMENT_FAILED;
    }
 }

 return IDLIB_SUCCESS;
}

static int
idlib_delete_regular_file
  (
    char const* pathname
  )
{
  BOOL result = DeleteFile(pathname);
  if (!result) {
    DWORD error = GetLastError();
    if (ERROR_ACCESS_DENIED == error || ERROR_FILE_NOT_FOUND == error) {
      return IDLIB_FILE_NOT_FOUND;
    } else {
      return IDLIB_ENVIRONMENT_FAILED;
    }
  }

  return IDLIB_SUCCESS;
}

#endif // IDLIB_OPERATING_SYSTEM_WINDOWS

int
idlib_delete_file
  (
    char const* path_name
  )
{
  if (!path_name) {
    return IDLIB_ARGUMENT_INVALID;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  idlib_file_type file_type;
  int result = idlib_get_file_type(path_name, &file_type);
  if (result) {
    return result;
  }

  if (file_type == idlib_file_type_directory) {
    return idlib_delete_directory_file(path_name);
  } else {
    return idlib_delete_regular_file(path_name);
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  if (!strcmp(path_name, "")) {
    // Because ENOENT is not only returned when the file is not found but also when path_name is empty.
    return IDLIB_ARGUMENT_INVALID;
  }
  if (-1 == remove(path_name)) {
    if (ENOENT == errno) {
      return IDLIB_FILE_NOT_FOUND;
    } else {
      return IDLIB_ENVIRONMENT_FAILED;
    }
  }

#else

  #error("operating system not (yet) supported")

#endif
}
