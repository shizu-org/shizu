/*
  IdLib File System
  Copyright (C) 2023-2024 Michael Heilmann. All rights reserved.

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

#include "idlib/file_system.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// fprintf, stderr
#include <stdio.h>

int
main
  (
    int argc,
    char** argv
  )
{
  idlib_file_type file_type;
  if (idlib_get_file_type("./assets/directory1", &file_type)) {
    fprintf(stderr, "%s:%d: test failed\n", __FILE__, __LINE__);
    return EXIT_FAILURE;
  }
  if (idlib_file_type_directory != file_type) {
    fprintf(stderr, "%s:%d: test failed\n", __FILE__, __LINE__);
    return EXIT_FAILURE;
  }
  if (idlib_get_file_type("./assets/directory1/file1.txt", &file_type)) {
    fprintf(stderr, "%s:%d: test failed\n", __FILE__, __LINE__);
    return EXIT_FAILURE;
  }
  if (idlib_file_type_regular != file_type) {
    fprintf(stderr, "%s:%d: test failed\n", __FILE__, __LINE__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

