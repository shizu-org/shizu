#include "Shizu/Runtime/Include.h"

// fprintf, stdio
#include <stdio.h>

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// strlen
#include <string.h>

static Shizu_ByteArray* getFileContents(Shizu_State2* state,  Shizu_String* relativePath) {
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_Environment* fileSystemEnvironment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
  Shizu_CxxProcedure* p = Shizu_Environment_getCxxProcedure(state, fileSystemEnvironment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type, Shizu_String_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* path = (Shizu_String*)Shizu_Value_getObject(&returnValue);
  // Get the directory separator.
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, fileSystemEnvironment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  path = Shizu_String_concatenate(state, path, directorySeparator);
  path = Shizu_String_concatenate(state, path, relativePath);
  // Get the file contents.
  p = Shizu_Environment_getCxxProcedure(state, fileSystemEnvironment, Shizu_String_create(state, "getFileContents", strlen("getFileContents")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  p->f(state, &returnValue, 1, &argumentValues[0]);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
                               Shizu_ByteArray_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ByteArray* received = (Shizu_ByteArray*)Shizu_Value_getObject(&returnValue);
  return received;
}

static void scan(Shizu_State2* state, Shizu_String* relativePath) {
  Shizu_ByteArray* inputByteArray = NULL;
  Shizu_String* inputString = NULL;
  inputByteArray = getFileContents(state, relativePath);
  inputString = Shizu_String_create(state, Shizu_ByteArray_getRawBytes(state, inputByteArray), Shizu_ByteArray_getNumberOfRawBytes(state, inputByteArray));
  Shizu_Environment* compilerEnvironment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "Compiler", strlen("Compiler")));
  Shizu_CxxProcedure* p = Shizu_Environment_getCxxProcedure(state, compilerEnvironment, Shizu_String_create(state, "createScanner", strlen("createScanner")));
  Shizu_Value returnValue; Shizu_Value arguments[1];
  p->f(state, &returnValue, 0, &arguments[0]);
  Shizu_Value self = returnValue;
  // set the input
  {
    Shizu_Value returnValue;
    Shizu_Value arguments[1];
    Shizu_Value_setObject(&arguments[0], (Shizu_Object*)inputString);
    uint8_t const* methodNameBytes = "setInput";
    Shizu_Object_call(state, Shizu_Value_getObject(&self), methodNameBytes, sizeof("setInput") - 1, &returnValue, 1, &arguments[0]);
  }
  // read tokens until the end of the input or an error is encountered
  {
    Shizu_Environment* tokenTypeEnvironment = Shizu_Environment_getEnvironment(state, Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "Compiler", sizeof("Compiler")-1)), Shizu_String_create(state, "TokenType", sizeof("TokenType") - 1));
    Shizu_Value tokenTypeStartOfInput;
    Shizu_Value_setInteger32(&tokenTypeStartOfInput, Shizu_Environment_getInteger32(state, tokenTypeEnvironment, Shizu_String_create(state, "StartOfInput", sizeof("StartOfInput") - 1)));
    Shizu_Value tokenTypeEndOfInput;
    Shizu_Value_setInteger32(&tokenTypeEndOfInput, Shizu_Environment_getInteger32(state, tokenTypeEnvironment, Shizu_String_create(state, "EndOfInput", sizeof("EndOfInput") - 1)));
    Shizu_Value tokenTypeError;
    Shizu_Value_setInteger32(&tokenTypeError, Shizu_Environment_getInteger32(state, tokenTypeEnvironment, Shizu_String_create(state, "Error", sizeof("Error") - 1)));


    Shizu_Value returnValue;
    Shizu_Value arguments[1];
    uint8_t const stepMethodNameBytes[] = "step";
    uint8_t const getTokenTypeMethodNameBytes[] = "getTokenType";
    do {
      Shizu_Object_call(state, Shizu_Value_getObject(&self), &stepMethodNameBytes[0], sizeof(stepMethodNameBytes) - 1, &returnValue, 0, &arguments[0]);
      Shizu_Object_call(state, Shizu_Value_getObject(&self), &getTokenTypeMethodNameBytes[0], sizeof(getTokenTypeMethodNameBytes) - 1, &returnValue, 0, &arguments[0]);
    } while (Shizu_Value_getInteger32(&returnValue) != Shizu_Value_getInteger32(&tokenTypeEndOfInput) &&
             Shizu_Value_getInteger32(&returnValue) != Shizu_Value_getInteger32(&tokenTypeError));
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  Shizu_State2* state = NULL;
  if (Shizu_State2_acquire(&state)) {
    return EXIT_FAILURE;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State2_ensureModulesLoaded(state);
    Shizu_Environment* fileSystemEnvironment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
    Shizu_String* directorySeparator = Shizu_Environment_getString(state, fileSystemEnvironment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
    Shizu_String* path = Shizu_String_create(state, "Assets", strlen("Assets"));
    path = Shizu_String_concatenate(state, path, directorySeparator);
    Shizu_ByteArray* inputByteArray = NULL;
    Shizu_String* input = NULL;
    //
    scan(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "HelloWorld1.cil", strlen("HelloWorld1.cil"))));
    //
    scan(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "HelloWorld2.cil", strlen("HelloWorld2.cil"))));
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_relinquish(state);
    state = NULL;
    return EXIT_FAILURE;
  }
  Shizu_State2_relinquish(state);
  state = NULL;
  return EXIT_SUCCESS;
}
