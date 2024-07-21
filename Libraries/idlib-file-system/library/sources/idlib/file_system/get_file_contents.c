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

#include "idlib/file_system/get_file_contents.h"

#include "idlib/file_system/file_handle.h"

#include "idlib/file_system/file_mapping.h"

// malloc, free
#include <malloc.h>

// SIZE_MAX
#include <stdlib.h>

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // For errno.
  #include <errno.h>

  // For open.
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>

  // For close.
  #include <unistd.h>

#else

  #error("operating system not (yet) supported")

#endif

int
idlib_get_file_contents
  (
    char const* path_name,
    void* context,
    idlib_get_file_contents_callback* callback
  )
{
  idlib_file_handle* file_handle = NULL;
  int result;
  result = idlib_file_handle_create(&file_handle,
                                    path_name,
                                    idlib_file_access_mode_read,
                                    idlib_existing_file_policy_retain,
                                    idlib_non_existing_file_policy_fail);
  if (result) {
    return result;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  while (true) {
    char temporary[5012];
    DWORD requested = 5012;
    DWORD received = 0;
    BOOL result = ReadFile(idlib_file_handle_get_backend(file_handle), temporary, requested, &received, NULL);
    if (!result) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ENVIRONMENT_FAILED;
    }
    // eof
    if (received == 0) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      break;
    }
    if (!(*callback)(context, temporary, received)) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ABORTED;
    }
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  while (true) {
    char temporary[5012];
    size_t requested = 5012;
    ssize_t received = 0;
    received = read(idlib_file_handle_get_backend(file_handle), temporary, requested);
    if (-1 == received) {
      errno = 0;
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ENVIRONMENT_FAILED;
    }
    // eof
    if (0 == received) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      break;
    }
    if (!(*callback)(context, temporary, (size_t)received)) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ABORTED;
    }
  }

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

int
idlib_get_file_contents_memory_mapped
  (
    char const* path_name,
    void* context,
    idlib_get_file_contents_callback* callback
  )
{
  int result;
  // Open the file mapping.
  idlib_file_mapping file_mapping;
  result = idlib_file_mapping_initialize_read(&file_mapping, path_name);
  if (result) {
    return result;
  }
  // Copy from source bytes to target bytes.
  if (!(*callback)(context, file_mapping.bytes, file_mapping.number_of_bytes)) {
    // Close file mapping.
    idlib_file_mapping_uninitialize(&file_mapping);
    return IDLIB_ABORTED;
  }
  // Close the file mapping.
  result = idlib_file_mapping_uninitialize(&file_mapping);
  if (result) {
    return result;
  }
  return IDLIB_SUCCESS;
}
