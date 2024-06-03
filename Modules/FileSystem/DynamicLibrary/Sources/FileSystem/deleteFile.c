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

#include "FileSystem/deleteFile.h"

#include "FileSystem/Utilities.h"
#include "idlib/file_system.h"

void
deleteFile
  (
    Shizu_State* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (!returnValue || !argumentValues || 1 != numberOfArgumentValues) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  Shizu_String* path = Shizu_Value_getStringArgument(state, argumentValues + 0);
  path = Shizu_toNativePath(state, path);
  idlib_status status = idlib_delete_file(Shizu_String_getBytes(state, path));
  if (IDLIB_SUCCESS != status && IDLIB_FILE_NOT_FOUND != status) {
    fprintf(stderr, "%s:%d: %s failed with %"PRIu32"\n", __FILE__, __LINE__, "idlib_delete_file", status);
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  // Return true if the file was deleted, return false if the file was not found.
  Shizu_Value_setBoolean(returnValue, IDLIB_SUCCESS == status);
}
