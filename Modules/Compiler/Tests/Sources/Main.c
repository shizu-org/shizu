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

static Shizu_ByteArray* getFileContents(Shizu_State* state,  Shizu_String* relativePath) {
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
  // @todo Add and use "getDirectorySeparator".
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
  return received;
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
    Shizu_Environment* environment = Shizu_State_getGlobals(state);
    Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
    Shizu_String* path = Shizu_String_create(state, "Assets", strlen("Assets"));
    path = Shizu_String_concatenate(state, path, directorySeparator);
    getFileContents(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "HelloWorld1.cil", strlen("HelloWorld1.cil"))));
    getFileContents(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "HelloWorld2.cil", strlen("HelloWorld2.cil"))));
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
