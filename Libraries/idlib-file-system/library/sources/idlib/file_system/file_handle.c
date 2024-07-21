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

#include "idlib/file_system/file_handle.h"

// malloc, free
#include <malloc.h>

// SIZE_MAX
#include <stdint.h>

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // For open.
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>

  // For close.
  #include <unistd.h>

#else

  #error("operating system not (yet) supported")

#endif

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  struct idlib_file_handle {
    HANDLE hFileHandle;
    idlib_file_access_mode file_access_mode;
  };

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  struct idlib_file_handle {
    int fd;
    idlib_file_access_mode file_access_mode;
  };

#else

  #error("operating system not (yet) supported")

#endif

int
idlib_file_handle_create
  (
    idlib_file_handle** file_handle,
    char const* pathname,
    idlib_file_access_mode file_access_mode,
    idlib_existing_file_policy existing_file_policy,
    idlib_non_existing_file_policy non_existing_file_policy
  )
{
  // Validate arguments.
  if (!file_handle || !pathname) {
    /*fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);*/
    return IDLIB_ARGUMENT_INVALID;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // Translate desired access and (non) existing file policies.
  DWORD dwDesiredAccess = 0;
  if (!(file_access_mode & (idlib_file_access_mode_read_write))) {
    /*fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);*/
    return IDLIB_ARGUMENT_INVALID;
  }
  if (file_access_mode & idlib_file_access_mode_read) {
    dwDesiredAccess |= GENERIC_READ;
  }
  if (file_access_mode & idlib_file_access_mode_write) {
    dwDesiredAccess |= GENERIC_WRITE;
  }
  DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
  DWORD dwCreationDisposition = 0;
  typedef struct POLICYMAPPING {
    idlib_existing_file_policy existing_file_policy;
    idlib_non_existing_file_policy non_existing_file_policy;
    DWORD dwCreationDisposition;
    DWORD dwDesiredAccessModifier;
  } POLICYMAPPING;
  static const POLICYMAPPING policy_mappings[] = {
    // CREATE_NEW is a conditional flag which fails
    // if the file exists and succeeds if the file
    // does not exist and is successfully created.
    // It is not used.
    { idlib_existing_file_policy_truncate,
      idlib_non_existing_file_policy_create,
      CREATE_ALWAYS,
      0 },
    { idlib_existing_file_policy_retain,
      idlib_non_existing_file_policy_create,
      OPEN_ALWAYS,
      0 },
    { idlib_existing_file_policy_truncate,
      idlib_non_existing_file_policy_fail,
      TRUNCATE_EXISTING,
      GENERIC_WRITE },
    { idlib_existing_file_policy_retain,
      idlib_non_existing_file_policy_fail,
      OPEN_EXISTING,
      0 },
  };
  const POLICYMAPPING *policy_mapping = NULL;
  for (size_t i = 0, n = sizeof(policy_mappings) / sizeof(POLICYMAPPING); i < n; ++i) {
    policy_mapping = policy_mappings + i;
    if (policy_mapping->existing_file_policy == existing_file_policy &&
        policy_mapping->non_existing_file_policy == non_existing_file_policy) {
      break;
    }
  }
  if (policy_mapping == NULL) {
    /*fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);*/
    return IDLIB_ARGUMENT_INVALID;
  }
  dwDesiredAccess |= policy_mapping->dwDesiredAccessModifier;
  idlib_file_handle *temporary_file_handle = malloc(sizeof(idlib_file_handle));
  if (!temporary_file_handle) {
     /*fprintf(stderr, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, sizeof(idlib_file_handle));*/
    return IDLIB_ALLOCATION_FAILED;
  }
  temporary_file_handle->hFileHandle = CreateFileA(pathname, dwDesiredAccess, dwShareMode, 0, policy_mapping->dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
  if (INVALID_HANDLE_VALUE == temporary_file_handle->hFileHandle) {
    free(temporary_file_handle);
    temporary_file_handle = NULL;
    /*fprintf(stderr, "%s:%d: unable to open file '%s'\n", __FILE__, __LINE__, path_name);*/
    return IDLIB_ENVIRONMENT_FAILED;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // Translate file access and policies for existing and non-existing files.
  int flags = 0;
  if (idlib_file_access_mode_read_write == (file_access_mode & (idlib_file_access_mode_read_write))) {
    flags |= O_RDWR;
  } else if (file_access_mode & idlib_file_access_mode_read) {
    flags |= O_RDONLY;
  } else if (file_access_mode & idlib_file_access_mode_write) {
    flags |= O_WRONLY;
  } else {
    /* fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__); */
    return IDLIB_ARGUMENT_INVALID;
  }
  typedef struct POLICYMAPPING
  {
      idlib_existing_file_policy existing_file_policy;
      idlib_non_existing_file_policy non_existing_file_policy;
      int flags_modifier;
  } POLICYMAPPING;
  static const POLICYMAPPING policy_mappings[] =
  {
    { idlib_existing_file_policy_truncate,
      idlib_non_existing_file_policy_create,
      O_CREAT | O_TRUNC },
    { idlib_existing_file_policy_retain,
      idlib_non_existing_file_policy_create,
      O_CREAT },
    { idlib_existing_file_policy_truncate,
      idlib_non_existing_file_policy_fail,
      O_TRUNC },
    { idlib_existing_file_policy_retain,
      idlib_non_existing_file_policy_fail,
      0 },
  };
  const POLICYMAPPING *policy_mapping = NULL;
  for (size_t i = 0, n = sizeof(policy_mappings) / sizeof(POLICYMAPPING); i < n; ++i) {
    policy_mapping = policy_mappings + i;
    if (policy_mapping->existing_file_policy == existing_file_policy &&
      policy_mapping->non_existing_file_policy == non_existing_file_policy) {
      break;
    }
  }
  if (NULL == policy_mapping) {
    /*fprintf(stderr, "%s:%d: invalid argument\n", __FILE__, __LINE__);*/
    return IDLIB_ARGUMENT_INVALID;
  }
  flags |= policy_mapping->flags_modifier;
  idlib_file_handle *temporary_file_handle = malloc(sizeof(idlib_file_handle));
  if (!temporary_file_handle) {
    return IDLIB_ALLOCATION_FAILED;
  }
  if (non_existing_file_policy == idlib_non_existing_file_policy_create) {
    temporary_file_handle->fd = open(pathname, flags, S_IRWXU);
  } else {
    temporary_file_handle->fd = open(pathname, flags);
  }
  if (temporary_file_handle->fd < 0) {
    free(temporary_file_handle);
    /*fprintf(stderr, "%s:%d: unable to open file '%s'\n", __FILE__, __LINE__, path_name);*/
    return IDLIB_ENVIRONMENT_FAILED;
  }

#else

  #error("operating system not (yet) supported")

#endif

  temporary_file_handle->file_access_mode = file_access_mode;

  *file_handle = temporary_file_handle;

  return IDLIB_SUCCESS;
}

int
idlib_file_handle_destroy
  (
    idlib_file_handle* file_handle
  )
{

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

   // Close file handle.
  if (INVALID_HANDLE_VALUE != file_handle->hFileHandle) {
    CloseHandle(file_handle->hFileHandle);
    file_handle->hFileHandle = INVALID_HANDLE_VALUE;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // Close file handle.
  if (file_handle->fd > 0) {
    close(file_handle->fd);
    file_handle->fd = -1;
  }

#else

  #error("operating system not (yet) supported")

#endif

  // Deallocate file handle.
  free(file_handle);
  file_handle = NULL;

  return IDLIB_SUCCESS;
}

int
idlib_file_handle_get_file_size
  (
    idlib_file_handle* file_handle,
    size_t* file_size
  )
{
  if (!file_handle || !file_size) {
    return IDLIB_ARGUMENT_INVALID;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  if (INVALID_HANDLE_VALUE == file_handle->hFileHandle) {
    return IDLIB_ARGUMENT_INVALID;
  }
  // Get the size of the file. The file size must be smaller than or equal to Core_Size_Greatest.
  DWORD temporary_file_size = GetFileSize(file_handle->hFileHandle, NULL);
  if (INVALID_FILE_SIZE == temporary_file_size || temporary_file_size > SIZE_MAX) {
    return IDLIB_TOO_BIG;
  }
  *file_size = temporary_file_size;

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  if (file_handle->fd < 0) {
    return IDLIB_ARGUMENT_INVALID;
  }
  // Get the size of the file. The file size must be smaller than or equal to SIZE_MAX.
  struct stat stat_buf;
  int status = fstat(file_handle->fd, &stat_buf);
  if (status < 0 || stat_buf.st_size < 0 || stat_buf.st_size > SIZE_MAX) {
    return IDLIB_ENVIRONMENT_FAILED;
  }
  *file_size = (size_t)stat_buf.st_size;

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  HANDLE

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  int

#else
  
  #error("operating system not (yet) supported")

#endif

idlib_file_handle_get_backend
  (
    idlib_file_handle* file_handle
  )
{

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  return file_handle->hFileHandle;

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  return file_handle->fd;

#else

  #error("operating system not (yet) supported")

#endif

}

idlib_file_access_mode
idlib_file_handle_get_file_access_mode
  (
    idlib_file_handle* file_handle  
  )
{ return file_handle->file_access_mode; }
