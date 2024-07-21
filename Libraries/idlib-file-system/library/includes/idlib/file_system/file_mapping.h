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

#if !defined(IDLIB_FILE_SYSTEM_FILE_MAPPING_H_INCLUDED)
#define IDLIB_FILE_SYSTEM_FILE_MAPPING_H_INCLUDED

#include "idlib/file_system/file_handle.h"

/* Helper to store information related to a file mapping. */
typedef struct idlib_file_mapping {
  idlib_file_handle* file_handle;
  
#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  HANDLE hFileMapping;

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  /* Intentionally empty. */

#else

  #error("operating system not (yet) supported")

#endif

  void* bytes;
  size_t number_of_bytes;

} idlib_file_mapping;

int
idlib_file_mapping_initialize_write
  (
    idlib_file_mapping* file_mapping,
    char const* path_name,
    size_t number_of_bytes
  );

int
idlib_file_mapping_initialize_read
  (
    idlib_file_mapping* file_mapping,
    char const* path_name
  );

int
idlib_file_mapping_uninitialize
  (
    idlib_file_mapping* file_mapping
  );

#endif // IDLIB_FILE_SYSTEM_FILE_MAPPING_H_INCLUDED
