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

#include "idlib_test_buffer.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// fprintf, stderr
#include <stdio.h>

// memcmp
#include <string.h>

typedef struct Context {
  int result;
  idlib_test_utilities_buffer buffer;
} Context;

static int
Context_initialize
  (
    Context* context
  )
{
  int result;
  result = idlib_test_utilities_buffer_initialize(&context->buffer);
  if (result) {
    return result;
  }
  context->result = IDLIB_SUCCESS;
  return IDLIB_SUCCESS;
}

static int
Context_uninitialize
  (
    Context* context
  )
{
  idlib_test_utilities_buffer_uninitialize(&context->buffer);
  return IDLIB_SUCCESS;
}

static int
Context_grow
  (
    Context* context,
    size_t n
  )
{
  return idlib_test_utilities_buffer_grow(&context->buffer, n);
}

static int
Context_append
  (
    Context* context,
    void const* p,
    size_t n
  )
{
  return idlib_test_utilities_buffer_append(&context->buffer, p, n);
}

static int
Context_clear
  (
    Context* context
  )
{
  return idlib_test_utilities_buffer_clear(&context->buffer);
}

static bool
callback
  (
    void* context,
    void const* p,
    size_t n
  )
{
  int result = Context_append((Context*)context, p, n);
  if (result) {
    ((Context*)context)->result = result;
    return false;
  }
  return true;
}

static int
test
  (
    char const* path_name,
    char const* p,
    size_t n,
    bool memory_mapped
  )
{
  if (!path_name || !p) {
    return IDLIB_ARGUMENT_INVALID;
  }
  int result;
  Context context;
  result = Context_initialize(&context);
  if (result) {
    return result;
  }
  if (memory_mapped) {
    result = idlib_get_file_contents_memory_mapped(path_name, &context, &callback);
  } else {
    result = idlib_get_file_contents(path_name, &context, &callback);
  }
  if (result) {
    Context_uninitialize(&context);
    return result;
  }
  if (context.buffer.size != n) {
    Context_uninitialize(&context);
    return IDLIB_ENVIRONMENT_FAILED;
  }
  if (0 != memcmp(context.buffer.elements, p, n)) {
    Context_uninitialize(&context);
    return IDLIB_ENVIRONMENT_FAILED;
  }
  Context_uninitialize(&context);
  return IDLIB_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  int result;
  
  Context context;
  
  {
    char const p[] = { 0 };
    size_t const n = 0;
    result = test("./assets/directory1/file1.txt", p, n, false);
    if (result) {
      return EXIT_FAILURE;
    }
    result = test("./assets/directory1/file1.txt", p, n, true);
    if (result) {
      return EXIT_FAILURE;
    }
  }
  {
    char const p[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd','!','\r','\n' };
    size_t const n = sizeof(p);
    result = test("./assets/directory1/file2.txt", p, n, false);
    if (result) {
      return EXIT_FAILURE;
    }
    result = test("./assets/directory1/file2.txt", p, n, true);
    if (result) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

