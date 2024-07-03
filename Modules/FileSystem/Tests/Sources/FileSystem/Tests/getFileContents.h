#if !defined(FILESYSTEM_TESTS_GETFILECONTENTS_H_INCLUDED)
#define FILESYSTEM_TESTS_GETFILECONTENTS_H_INCLUDED

#include "Shizu/Runtime/Include.h"

void
FileSystem_Tests_testGetFileContents
  (
    Shizu_State2* state,
    Shizu_String* relativePath,
    Shizu_ByteArray* expected
  );

#endif // FILESYSTEM_TESTS_GETFILECONTENTS_H_INCLUDED

