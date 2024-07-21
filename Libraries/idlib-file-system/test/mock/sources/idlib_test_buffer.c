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

#include "idlib_test_buffer.h"

// malloc, realloc, free
#include <malloc.h>

// SIZE_MAX
#include <stdint.h>

// memcpy
#include <string.h>

int
idlib_test_utilities_buffer_initialize
  (
    idlib_test_utilities_buffer* self
  )
{
  self->elements = malloc(sizeof(char) * 8);
  if (!self->elements) {
    return IDLIB_ALLOCATION_FAILED;
  }
  self->size = 0;
  self->capacity = 8;
  self->result = IDLIB_SUCCESS;
  return IDLIB_SUCCESS;
}

int
idlib_test_utilities_buffer_uninitialize
  (
    idlib_test_utilities_buffer* self
  )
{
  free(self->elements);
  self->elements = NULL;
  return IDLIB_SUCCESS;
}

int
idlib_test_utilities_buffer_grow
  (
    idlib_test_utilities_buffer* context,
    size_t n
  )
{
  if (SIZE_MAX - context->capacity < n) {
    return IDLIB_ALLOCATION_FAILED;
  }
  size_t new_capacity = context->capacity + n;
  void* new_elements = realloc(context->elements, new_capacity);
  if (!new_elements) {
    return IDLIB_ALLOCATION_FAILED;
  }
  context->elements = new_elements;
  context->capacity = new_capacity;
  return IDLIB_SUCCESS;
}

int
idlib_test_utilities_buffer_append
  (
    idlib_test_utilities_buffer* context,
    void const* p,
    size_t n
  )
{
  size_t free_capacity = context->capacity - context->size;
  if (free_capacity < n) {
    size_t additional_free_capacity = n - free_capacity;
    int result = idlib_test_utilities_buffer_grow(context, additional_free_capacity);
    if (result) {
      return result;
    }
  }
  memcpy(((char*)context->elements) + context->size, p, n);
  context->size += n;
  return IDLIB_SUCCESS;
}

int
idlib_test_utilities_buffer_clear
  (
    idlib_test_utilities_buffer* context
  )
{
  context->size = 0;
  return IDLIB_SUCCESS;
}
