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

#if !defined(IDLIB_FILE_SYSTEM_DELETE_FILE_H_INCLUDED)
#define IDLIB_FILE_SYSTEM_DELETE_FILE_H_INCLUDED

/**
 * @brief Delete a file.
 * @param path_name The pathname to the directory file.
 * @return #IDLIB_SUCCESS on success. A non-zero value on failure.
 * In particular, this function returns #IDLIB_FS_FILE_NOT_FOUND if the directory file was not found.
 * This function fails if the directory is not empty.
 */
int
idlib_delete_file
  (
    char const* path_name
  );

#endif // IDLIB_FILE_SYSTEM_DELETE_FILE_H_INCLUDED
