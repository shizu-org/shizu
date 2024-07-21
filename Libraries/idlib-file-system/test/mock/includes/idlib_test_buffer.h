/*
  IdLib FileSystem
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

#if !defined(IDLIB_TEST_UTILITIES_BUFFER_H_INCLUDED)
#define IDLIB_TEST_UTILITIES_BUFFER_H_INCLUDED

#include "idlib/file_system.h"

typedef struct idlib_test_utilities_buffer {
  int result;
  void* elements;
  size_t size, capacity;
} idlib_test_utilities_buffer;

int
idlib_test_utilities_buffer_initialize
  (
    idlib_test_utilities_buffer* self
  );

int
idlib_test_utilities_buffer_uninitialize
  (
    idlib_test_utilities_buffer* self
  );

int
idlib_test_utilities_buffer_grow
  (
    idlib_test_utilities_buffer* self,
    size_t n
  );

int
idlib_test_utilities_buffer_append
  (
    idlib_test_utilities_buffer* self,
    void const* p,
    size_t n
  );

int
idlib_test_utilities_buffer_clear
  (
    idlib_test_utilities_buffer* self
  );

#endif // IDLIB_TEST_UTILITIES_BUFFER_H_INCLUDED
