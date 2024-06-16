#include "Shizu/Runtime/Include.h"

// fprintf, stdio
#include <stdio.h>

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// strlen
#include <string.h>

static Shizu_Object* Shizu_Environment_getObject(Shizu_State* state, Shizu_Environment* self, Shizu_String* name, Shizu_Type* type) {
  Shizu_Value v = Shizu_Environment_get(state, self, name);
  if (!Shizu_Value_isObject(&v)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(state), Shizu_State_getTypes(state), Shizu_Value_getObject(&v)->type, type)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  return Shizu_Value_getObject(&v);
}

static Shizu_CxxProcedure* Shizu_Environment_getCxxProcedure(Shizu_State* state, Shizu_Environment* self, Shizu_String* name) {
  Shizu_Type* type = Shizu_CxxProcedure_getType(state);
  return (Shizu_CxxProcedure*)Shizu_Environment_getObject(state, self, name, type);
}

static Shizu_String* Shizu_Environment_getString(Shizu_State* state, Shizu_Environment* self, Shizu_String* name) {
  return (Shizu_String*)Shizu_Environment_getObject(state, self, name, Shizu_String_getType(state));
}

static void testSetFileContents1(Shizu_State* state, Shizu_String* relativePath, Shizu_ByteArray* contents) {
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_CxxProcedure* p;
  Shizu_Environment* environment = Shizu_State_getGlobals(state);
  // Compute the path.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(state), Shizu_State_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
    Shizu_String_getType(state))) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
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
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(state), Shizu_State_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
                                                                                          Shizu_ByteArray_getType(state))) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  Shizu_ByteArray* received = (Shizu_ByteArray*)Shizu_Value_getObject(&returnValue);
  // Compare the written and the received file contents.
  if (!Shizu_ByteArray_compareRawBytes(state, received, contents)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
}

static void testSetFileContentsEmpty(Shizu_State* state) {
  Shizu_ByteArray* contents = Shizu_ByteArray_create(state);
  testSetFileContents1(state, Shizu_String_create(state, "output-empty.txt", strlen("output-empty.txt")), contents);
}

static void testSetFileContentsNonEmpty(Shizu_State* state) {
  Shizu_ByteArray* contents = Shizu_ByteArray_create(state);
  Shizu_ByteArray_apppendRawBytes(state, contents, "Hello, World!", strlen("Hello, World!"));
  testSetFileContents1(state, Shizu_String_create(state, "output-non-empty.txt", strlen("output-non-empty.txt")), contents);
}

static void testGetFileContents(Shizu_State* state,  Shizu_String* relativePath, Shizu_ByteArray* expected) {
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_Environment* environment = Shizu_State_getGlobals(state);
  Shizu_CxxProcedure* p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(state), Shizu_State_getTypes(state), Shizu_Value_getObject(&returnValue)->type, Shizu_String_getType(state))) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  Shizu_String* path = (Shizu_String*)Shizu_Value_getObject(&returnValue);
  // Get the directory separator.
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  path = Shizu_String_concatenate(state, path, directorySeparator);
  path = Shizu_String_concatenate(state, path, relativePath);
  // Get the file contents.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getFileContents", strlen("getFileContents")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  p->f(state, &returnValue, 1, &argumentValues[0]);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(state), Shizu_State_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
    Shizu_ByteArray_getType(state))) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
  Shizu_ByteArray* received = (Shizu_ByteArray*)Shizu_Value_getObject(&returnValue);
  // Compare the written and the received file contents.
  if (!Shizu_ByteArray_compareRawBytes(state, received, expected)) {
    Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State_jump(state);
  }
}

static void testGetFileContentsEmpty(Shizu_State* state) {
  Shizu_Environment* environment = Shizu_State_getGlobals(state);
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  Shizu_String* relativePath = Shizu_String_create(state, "Assets", strlen("Assets"));
  relativePath = Shizu_String_concatenate(state, relativePath, directorySeparator);
  relativePath = Shizu_String_concatenate(state, relativePath, Shizu_String_create(state, "empty-file.txt", strlen("empty-file.txt")));
  Shizu_ByteArray* expected = Shizu_ByteArray_create(state);
  testGetFileContents(state, relativePath, expected);
}

static void testGetFileContentsNonEmpty(Shizu_State* state) {
  Shizu_Environment* environment = Shizu_State_getGlobals(state);
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  Shizu_String* relativePath = Shizu_String_create(state, "Assets", strlen("Assets"));
  relativePath = Shizu_String_concatenate(state, relativePath, directorySeparator);
  relativePath = Shizu_String_concatenate(state, relativePath, Shizu_String_create(state, "non-empty-file.txt", strlen("non-empty-file.txt")));
  Shizu_ByteArray* expected = Shizu_ByteArray_create(state);
  Shizu_ByteArray_apppendRawBytes(state, expected, "Hello, World!", strlen("Hello, World!"));
  testGetFileContents(state, relativePath, expected);
}

int main(int argc, char** argv) {
  Shizu_State* state = NULL;
  if (Shizu_State_create(&state)) {
    return EXIT_FAILURE;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State_ensureModulesLoaded(state);
    (*testGetFileContentsEmpty)(state);
    (*testGetFileContentsNonEmpty)(state);
    (*testSetFileContentsEmpty)(state);
    (*testSetFileContentsNonEmpty)(state);
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_destroy(state);
    state = NULL;
    return EXIT_FAILURE;
  }
  Shizu_State_destroy(state);
  state = NULL;
  return EXIT_SUCCESS;
}
