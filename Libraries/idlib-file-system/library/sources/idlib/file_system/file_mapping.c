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

#include "idlib/file_system/file_mapping.h"

#if IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
    IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

// For mmap.
#include <sys/mman.h>

// For write, lseek, and SEEK_SET.
#include <unistd.h>

#endif

static const char DUMMY = 0;

int
idlib_file_mapping_initialize_write
  (
    idlib_file_mapping* file_mapping,
    char const* path_name,
    size_t number_of_bytes
  )
{
  int result;

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  file_mapping->hFileMapping = NULL;

#endif

  // Create the file handle.
  result = idlib_file_handle_create(&file_mapping->file_handle,
                                    path_name,
                                    idlib_file_access_mode_read_write,
                                    idlib_existing_file_policy_truncate,
                                    idlib_non_existing_file_policy_create);
  if (result) {
    return result;
  }
  // Set the number of Bytes.
  file_mapping->number_of_bytes = number_of_bytes;
  // If the file is empty, use a dummy buffer.
  if (0 == file_mapping->number_of_bytes) {
    file_mapping->bytes = (char*)&DUMMY;
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return the result.
    return IDLIB_SUCCESS;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // Create file mapping.
  file_mapping->hFileMapping = CreateFileMapping(idlib_file_handle_get_backend(file_mapping->file_handle), 0, PAGE_READWRITE, 0, number_of_bytes, NULL);
  if (NULL == file_mapping->hFileMapping) {
    /*fprintf(stderr, "unable to create file mapping of file '%s'\n", path_name);*/
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return the result.
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Create view of file mapping.
  file_mapping->bytes = (char*)MapViewOfFile(file_mapping->hFileMapping, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
  if (!file_mapping->bytes) {
    /*fprintf(stderr, "unable to create view of file '%s'\n", path_name);*/
    // Close file mapping.
    CloseHandle(file_mapping->hFileMapping);
    file_mapping->hFileMapping = NULL;
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return the result.
    return IDLIB_ENVIRONMENT_FAILED;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // Extend the file to the size of the contents if necessary.
  if (-1 == lseek(idlib_file_handle_get_backend(file_mapping->file_handle), number_of_bytes - 1, SEEK_SET)) {
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (-1 == write(idlib_file_handle_get_backend(file_mapping->file_handle), "", 1)) {
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Create the mapping.
  file_mapping->bytes = mmap(0, file_mapping->number_of_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, idlib_file_handle_get_backend(file_mapping->file_handle), 0);
  if (MAP_FAILED == file_mapping->bytes) {
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }

#else

  #error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

int
idlib_file_mapping_initialize_read
  (
    idlib_file_mapping* file_mapping,
    char const* path_name
  )
{
  int result;

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  file_mapping->hFileMapping = NULL;

#endif

  file_mapping->file_handle = NULL;

  // Create the file handle.
  result = idlib_file_handle_create(&file_mapping->file_handle,
                                    path_name,
                                    idlib_file_access_mode_read,
                                    idlib_existing_file_policy_retain,
                                    idlib_non_existing_file_policy_fail);
  if (result) {
    return result;
  }
  // Get the size of the file.
  result = idlib_file_handle_get_file_size(file_mapping->file_handle, &file_mapping->number_of_bytes);
  if (result) {
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return result;
  }
  // If the file is empty, use a dummy buffer.
  if (0 == file_mapping->number_of_bytes) {
    file_mapping->bytes = (char*)&DUMMY;
    // Close file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return success.
    return IDLIB_SUCCESS;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // Create file mapping.
  file_mapping->hFileMapping = CreateFileMapping(idlib_file_handle_get_backend(file_mapping->file_handle), 0, PAGE_READONLY, 0, file_mapping->number_of_bytes, NULL);
  if (NULL == file_mapping->hFileMapping) {
    DWORD dwError = GetLastError();
    /*fprintf(stderr, "unable to create file mapping of file '%s'\n", path_name);*/
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }
  // Create view of file mapping.
  file_mapping->bytes = (char*)MapViewOfFile(file_mapping->hFileMapping, FILE_MAP_READ, 0, 0, 0);
  if (!file_mapping->bytes) {
    /*fprintf(stderr, "unable to create view of file '%s'\n", path_name);*/
    // Close file mapping.
    CloseHandle(file_mapping->hFileMapping);
    file_mapping->hFileMapping = NULL;
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // Open the mapping.
  file_mapping->bytes = mmap(NULL, file_mapping->number_of_bytes, PROT_READ, MAP_PRIVATE, idlib_file_handle_get_backend(file_mapping->file_handle), 0);
  if (MAP_FAILED == file_mapping->bytes) {
    /*fprintf(stderr, "unable to open file '%s'\n", path_name);*/
    // Destroy file handle.
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
    // Return failure.
    return IDLIB_ENVIRONMENT_FAILED;
  }

#else

  #error("environment not yet supported")

#endif

  return IDLIB_SUCCESS;
}

int
idlib_file_mapping_uninitialize
  (
    idlib_file_mapping* file_mapping
  )
{

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  // Close view of file.
  if (&DUMMY != file_mapping->bytes) {
    UnmapViewOfFile(file_mapping->bytes);
    file_mapping->bytes = NULL;
  }
  // Close file mapping.
  if (NULL != file_mapping->hFileMapping) {
    CloseHandle(file_mapping->hFileMapping);
    file_mapping->hFileMapping = NULL;
  }

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  // Synchronize if we have a file handle (that is, when the number of Bytes is greater than 0) and if we are in write mode.
  if (file_mapping->file_handle) {
    if (idlib_file_access_mode_write == (idlib_file_access_mode_write & idlib_file_handle_get_file_access_mode(file_mapping->file_handle))) {
      msync(file_mapping->bytes, file_mapping->number_of_bytes, MS_SYNC);
    }
  }
  // Close file mapping.
  if (&DUMMY != file_mapping->bytes) {
    munmap(file_mapping->bytes, file_mapping->number_of_bytes);
  }

#else

  #error("environment not yet supported")

#endif

  // Close file handle.
  if (file_mapping->file_handle) {
    idlib_file_handle_destroy(file_mapping->file_handle);
    file_mapping->file_handle = NULL;
  }

  return IDLIB_SUCCESS;
}
