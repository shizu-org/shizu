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

#include "FileSystem/getWorkingDirectory.h"

#include "Shizu/Cxx/Include.h"
#include "Shizu/Runtime/Include.h"
#include "idlib/file_system.h"

typedef struct Context {
  Shizu_State2* state;
  Shizu_String* path;
} Context;

static bool
callback
  (
    Context* context,
    void const* bytes,
    size_t numberOfBytes
  )
{
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(context->state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    context->path = Shizu_String_create(context->state, bytes, numberOfBytes);
  }
  Shizu_State2_popJumpTarget(context->state);
  return true;
}

void
getWorkingDirectory
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Cxx_Debug_assert(NULL != state);
  Shizu_Cxx_Debug_assert(NULL != returnValue);
  Shizu_Cxx_Debug_assert(NULL != argumentValues);
  if (0 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Context context = {
    .state = state,
    .path = NULL,
  };
  if (idlib_get_working_directory(&context, (idlib_get_working_directory_callback*)&callback)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, (Shizu_Object*)context.path);
}

