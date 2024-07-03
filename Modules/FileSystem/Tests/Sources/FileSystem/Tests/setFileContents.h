#if !defined(FILESYSTEM_TESTS_SETFILECONTENTS_H_INCLUDED)
#define FILESYSTEM_TESTS_SETFILECONTENTS_H_INCLUDED

#include "Shizu/Runtime/Include.h"

void
FileSystem_Tests_testSetFileContents
  (
    Shizu_State2* state,
    Shizu_String* relativePath,
    Shizu_ByteArray* contents
  );

#endif // FILESYSTEM_TESTS_SETFILECONTENTS_H_INCLUDED

