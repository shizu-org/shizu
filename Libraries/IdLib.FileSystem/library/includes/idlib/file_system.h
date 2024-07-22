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

#if !defined(IDLIB_FILE_SYSTEM_H_INCLUDED)
#define IDLIB_FILE_SYSTEM_H_INCLUDED

#include "idlib/file_system/configure.h"
#include "idlib/file_system/create_directory_file.h"
#include "idlib/file_system/delete_file.h"
#include "idlib/file_system/enumerate_files.h"
#include "idlib/file_system/existing_file_policy.h"
#include "idlib/file_system/file_access_mode.h"
#include "idlib/file_system/get_file_contents.h"
#include "idlib/file_system/get_file_type.h"
#include "idlib/file_system/get_working_directory.h"
#include "idlib/file_system/non_existing_file_policy.h"
#include "idlib/file_system/set_file_contents.h"
#include "idlib/file_system/status.h"

#endif // IDLIB_FILE_SYSTEM_H_INCLUDED
