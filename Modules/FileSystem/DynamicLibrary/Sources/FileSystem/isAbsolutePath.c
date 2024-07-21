/*
  Shizu
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

#include "FileSystem/isAbsolutePath.h"

#include "FileSystem/Utilities.h"
#include "idlib/file_system.h"

static bool
isDriveLetter
  (
    char character
  )
{
  return ('a' <= character && character <= 'z')
      || ('A' <= character && character <= 'z')
       ;
}

void
isAbsolutePath
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);  
  }
  Shizu_String* path = Shizu_Value_getStringArgument(state, argumentValues + 0);
  char const *p = Shizu_String_getBytes(state, path);
  size_t n = Shizu_String_getNumberOfBytes(state, path);
#if Shizu_Configuration_OperatingSystem == Shizu_Configuration_OperatingSystem_Windows
  // https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
  if (n < 3) {
    // An absolute path must start with a drive letter followed by a colon followed by a backslash.
    Shizu_Value_setBoolean(returnValue, Shizu_Boolean_False);
    return;
  }
  if (!isDriveLetter(p[0])) {
    // "Volume designators (drive letters) are [...] case-insensitive. For example, "D:\" and "d:\" refer to the same volume."
    // https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file    
    Shizu_Value_setBoolean(returnValue, Shizu_Boolean_False);
    return;
  }
  if (!('\\' == p[1])) {
    Shizu_Value_setBoolean(returnValue, Shizu_Boolean_False);
    return;    
  }
#else
  if (n < 1) {
    Shizu_Value_setBoolean(returnValue, Shizu_Boolean_False);
    return;
  }
  if (! ('/'  == p[0])) {
    Shizu_Value_setBoolean(returnValue, Shizu_Boolean_False);
    return;
  }
#endif

  Shizu_Value_setBoolean(returnValue, Shizu_Boolean_True);
}

