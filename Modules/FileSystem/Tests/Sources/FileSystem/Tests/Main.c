#include "Shizu/Runtime/Include.h"

#include "FileSystem/Tests/getFileContents.h"
#include "FileSystem/Tests/setFileContents.h"

// fprintf, stdio
#include <stdio.h>

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// strlen
#include <string.h>


static void testSetFileContentsEmpty(Shizu_State2* state) {
  Shizu_ByteArray* contents = Shizu_ByteArray_create(state);
  FileSystem_Tests_testSetFileContents(state, Shizu_String_create(state, "output-empty.txt", strlen("output-empty.txt")), contents);
}

static void testSetFileContentsNonEmpty(Shizu_State2* state) {
  Shizu_ByteArray* contents = Shizu_ByteArray_create(state);
  Shizu_ByteArray_appendRawBytes(state, contents, "Hello, World!", strlen("Hello, World!"));
  FileSystem_Tests_testSetFileContents(state, Shizu_String_create(state, "output-non-empty.txt", strlen("output-non-empty.txt")), contents);
}

static void testGetFileContentsEmpty(Shizu_State2* state) {
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  Shizu_String* relativePath = Shizu_String_create(state, "Assets", strlen("Assets"));
  relativePath = Shizu_String_concatenate(state, relativePath, directorySeparator);
  relativePath = Shizu_String_concatenate(state, relativePath, Shizu_String_create(state, "empty-file.txt", strlen("empty-file.txt")));
  Shizu_ByteArray* expected = Shizu_ByteArray_create(state);
  FileSystem_Tests_testGetFileContents(state, relativePath, expected);
}

static void testGetFileContentsNonEmpty(Shizu_State2* state) {
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  Shizu_String* relativePath = Shizu_String_create(state, "Assets", strlen("Assets"));
  relativePath = Shizu_String_concatenate(state, relativePath, directorySeparator);
  relativePath = Shizu_String_concatenate(state, relativePath, Shizu_String_create(state, "non-empty-file.txt", strlen("non-empty-file.txt")));
  Shizu_ByteArray* expected = Shizu_ByteArray_create(state);
  Shizu_ByteArray_appendRawBytes(state, expected, "Hello, World!", strlen("Hello, World!"));
  FileSystem_Tests_testGetFileContents(state, relativePath, expected);
}

int main(int argc, char** argv) {
  Shizu_State2* state = NULL;
  if (Shizu_State2_acquire(&state)) {
    return EXIT_FAILURE;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State2_ensureModulesLoaded(state);
    (*testGetFileContentsEmpty)(state);
    (*testGetFileContentsNonEmpty)(state);
    (*testSetFileContentsEmpty)(state);
    (*testSetFileContentsNonEmpty)(state);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_relinquish(state);
    state = NULL;
    return EXIT_FAILURE;
  }
  Shizu_State2_relinquish(state);
  state = NULL;
  return EXIT_SUCCESS;
}
