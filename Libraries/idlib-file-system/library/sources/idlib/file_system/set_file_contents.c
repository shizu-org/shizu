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

#include "idlib/file_system/set_file_contents.h"

#include "idlib/file_system/file_mapping.h"
#include "idlib/file_system/status.h"

// For memcpy.
#include <string.h>

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // For write and ssize_t.
  #include <unistd.h>

#else

  #error("operating system not (yet) supported")

#endif

int
idlib_set_file_contents
  (
    char const* path_name,
    void const* p,
    size_t n
  )
{
  if (!path_name || !p) {
    return IDLIB_ARGUMENT_INVALID;
  }
  int result;
  idlib_file_handle* file_handle = NULL;
  result = idlib_file_handle_create(&file_handle,
                                    path_name,
                                    idlib_file_access_mode_read_write,
                                    idlib_existing_file_policy_truncate,
                                    idlib_non_existing_file_policy_create);
  if (result) {
    return result;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  if (n > MAXDWORD) {
    return IDLIB_ARGUMENT_INVALID;
  }
  DWORD bytes_to_write_dw = n;
  DWORD bytes_written_dw = 0;
  while (bytes_written_dw < bytes_to_write_dw) {
    DWORD bytes_written_now_dw;
    DWORD bytes_to_write_now_dw = bytes_to_write_dw - bytes_written_dw;
    if (!WriteFile(idlib_file_handle_get_backend(file_handle), ((char const*)p) + bytes_written_dw, bytes_to_write_dw, &bytes_written_now_dw, NULL)) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ENVIRONMENT_FAILED;
    }
    bytes_written_dw += bytes_written_now_dw;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  ssize_t bytes_to_write = n;
  ssize_t bytes_written = 0;
  while (bytes_to_write) {
    ssize_t written_now = write(idlib_file_handle_get_backend(file_handle), ((char const*)p) + bytes_written, bytes_to_write);
    if (-1 == written_now) {
      idlib_file_handle_destroy(file_handle);
      file_handle = NULL;
      return IDLIB_ENVIRONMENT_FAILED;
    }
    bytes_written += written_now;
    bytes_to_write -= written_now;
  }

#else

  #error("environment not yet supported")

#endif

  idlib_file_handle_destroy(file_handle);
  file_handle = NULL;
  return IDLIB_SUCCESS;
}

int
idlib_set_file_contents_memory_mapped
  (
    char const* path_name,
    void const* p,
    size_t n
  )
{
  if (!path_name || !p) {
    /*fprintf(stderr, "invalid arguments\n");*/
    return IDLIB_ARGUMENT_INVALID;
  }
  int result;
  idlib_file_mapping file_mapping;
  //
  result = idlib_file_mapping_initialize_write(&file_mapping, path_name, n);
  if (result) {
    return result;
  }
  // Write the Bytes.
  memcpy(file_mapping.bytes, p, n);
  //
  idlib_file_mapping_uninitialize(&file_mapping);
  // Return the result.
  return IDLIB_SUCCESS;
}
