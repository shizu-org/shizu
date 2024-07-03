#include "setFileContents.h"

// strlen
#include <string.h>

void
FileSystem_Tests_testSetFileContents
  (
    Shizu_State2* state,
    Shizu_String* relativePath,
    Shizu_ByteArray* contents
  )
{
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_CxxProcedure* p;
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
  // Compute the path.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
                               Shizu_String_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* path = (Shizu_String*)Shizu_Value_getObject(&returnValue);
  // Get the directory separator.
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  path = Shizu_String_concatenate(state, path, directorySeparator);
  path = Shizu_String_concatenate(state, path, relativePath);
  // Delete the file if it exists.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "deleteFile", strlen("deleteFile")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  p->f(state, &returnValue, 1, &argumentValues[0]);
  // Create and write the file.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "setFileContents", strlen("setFileContents")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  Shizu_Value_setObject(&argumentValues[1], (Shizu_Object*)contents);
  p->f(state, &returnValue, 2, &argumentValues[0]);
  // Get the file contents.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getFileContents", strlen("getFileContents")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  p->f(state, &returnValue, 1, &argumentValues[0]);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
                               Shizu_ByteArray_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ByteArray* received = (Shizu_ByteArray*)Shizu_Value_getObject(&returnValue);
  // Compare the written and the received file contents.
  if (!Shizu_ByteArray_compareRawBytes(state, received, contents)) {
    Shizu_State2_setStatus(state, Shizu_Status_RuntimeTestFailed);
    Shizu_State2_jump(state);
  }
}

