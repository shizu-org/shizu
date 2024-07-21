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

#if !defined(IDLIB_FILE_SYSTEM_FILE_HANDLE_H_INCLUDED)
#define IDLIB_FILE_SYSTEM_FILE_HANDLE_H_INCLUDED

#include "idlib/file_system.h"

// size_t
#include <stddef.h>

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif

typedef struct idlib_file_handle idlib_file_handle;

int
idlib_file_handle_create
  (
    idlib_file_handle** file_handle,
    char const* pathname,
    idlib_file_access_mode file_access_mode,
    idlib_existing_file_policy existing_file_policy,
    idlib_non_existing_file_policy non_existing_file_policy
  );

int
idlib_file_handle_destroy
  (
    idlib_file_handle* file_handle
  );

int
idlib_file_handle_get_file_size
  (
    idlib_file_handle* file_handle,
    size_t* file_size
  );

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

  HANDLE

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

  int

#else

  #error("operating system not (yet) supported")

#endif

idlib_file_handle_get_backend
  (
    idlib_file_handle* file_handle
  );

idlib_file_access_mode
idlib_file_handle_get_file_access_mode
  (
    idlib_file_handle* file_handle
  );

#endif // IDLIB_FILE_SYSTEM_FILE_HANDLE_H_INCLUDED
