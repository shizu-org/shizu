/*
  Shizu Runtime
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

#include "FileSystem/setFileContents.h"

#include "FileSystem/Utilities.h"
#include "idlib/file_system.h"

// fprintf, stderr
#include <stdio.h>

void
setFileContents
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (!returnValue || 2 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* path = Shizu_Value_getStringArgument(state, argumentValues + 0);
  path = Shizu_toNativePath(state, path);
  Shizu_ByteArray* byteArray = Shizu_Value_getByteArrayArgument(state, argumentValues + 1);
  Shizu_Value value = Shizu_ByteArray_getSize(state, byteArray);
  idlib_status status = idlib_set_file_contents_memory_mapped(Shizu_String_getBytes(state, path), Shizu_ByteArray_getRawBytes(state, byteArray), Shizu_ByteArray_getNumberOfRawBytes(state, byteArray));
  if (status) {
    fprintf(stderr, "%s:%d: %s failed with %"PRIu32"\n", __FILE__, __LINE__, "idlib_set_file_contents_memory_mapped", status);
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
}
