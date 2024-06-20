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

#include "FileSystem/Utilities.h"

#include "idlib/byte_sequence.h"

Shizu_ByteArray*
Shizu_Value_getByteArrayArgument
  (
    Shizu_State2* state,
    Shizu_Value* value
  )
{
  Shizu_Type* type = Shizu_ByteArray_getType(state);
  if (!type) {
    // Cannot be a Shizu.ByteArray as no value of that type was instantiate anyway.
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_State_getObjectType(state, Shizu_Value_getObject(value)), type)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  return (Shizu_ByteArray*)Shizu_Value_getObject(value);
}

Shizu_String*
Shizu_Value_getStringArgument
  (
    Shizu_State2* state,
    Shizu_Value* value
  )
{
  Shizu_Type* type = Shizu_String_getType(state);
  if (!type) {
    // Cannot be a Shizu.String as no value of that type was instantiate anyway.
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isObject(value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Object* object = Shizu_Value_getObject(value);
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_State_getObjectType(state, object), type)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  return (Shizu_String*)Shizu_Value_getObject(value);
}

Shizu_String*
Shizu_toNativePath
  (
    Shizu_State2* state,
    Shizu_String* path
  )
{
  Shizu_String* zeroTerminator = Shizu_String_create(state, "", 1);
  bool found;
  size_t index;
  idlib_status status = idlib_byte_sequence_find(Shizu_String_getBytes(state, path), Shizu_String_getNumberOfBytes(state, path),
                                                 Shizu_String_getBytes(state, zeroTerminator), Shizu_String_getNumberOfBytes(state, zeroTerminator),
                                                 &found, &index);
  if (status) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  if (found) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  path = Shizu_String_concatenate(state, path, zeroTerminator);
  return path;
}
