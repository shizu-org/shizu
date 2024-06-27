#include "Shizu/Runtime/Include.h"

// fprintf, stdio
#include <stdio.h>

// strlen
#include <string.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export
#else
  #error("operating system not (yet) supported")
#endif

#include "Compiler/Parser.h"
#include "Compiler/Scanner.h"

/* Must not be "static". dladr fails otherwise. */ void
createScanner
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (0 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, (Shizu_Object*)Compiler_Scanner_create(state));
}

/* Must not be "static". dladr fails otherwise. */ void
createParser
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (0 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, (Shizu_Object*)Compiler_Parser_create(state));
}

static Shizu_Environment*
getOrCreateEnvironment
  (
    Shizu_State2* state,
    Shizu_Environment* environment,
    Shizu_String* name
  )
{
  if (!Shizu_Environment_isDefined(state, environment, name)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_Environment_create(state));
    Shizu_Environment_set(state, environment, name, &temporary);
  }
  environment = Shizu_Environment_getEnvironment(state, environment, name);
  return environment;
}

Shizu_Module_Export void
Shizu_ModuleLibrary_load
  (
    Shizu_State2* state
  )
{
  Shizu_Dl* dl = NULL;
  Shizu_JumpTarget jumpTarget;

  Shizu_Environment* environment = NULL;
  environment = getOrCreateEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "Compiler", strlen("Compiler")));

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State1_getDlByAdr(Shizu_State2_getState1(state), &createScanner);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &createScanner, dl));
    Shizu_Environment_set(state, environment, Shizu_String_create(state, "createScanner", strlen("createScanner")), &value);
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
  } else {
    Shizu_State2_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
    }
    Shizu_State2_jump(state);
  }

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State1_getDlByAdr(Shizu_State2_getState1(state), &createParser);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &createParser, dl));
    Shizu_Environment_set(state, environment, Shizu_String_create(state, "createParser", strlen("createParser")), &value);
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
  } else {
    Shizu_State2_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
    }
    Shizu_State2_jump(state);
  }

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Environment* environment1 = getOrCreateEnvironment(state, environment, Shizu_String_create(state, "TokenType", strlen("TokenType")));
    Shizu_Value value;
#define Define(Name) \
    Shizu_Value_setInteger32(&value, Compiler_TokenType_##Name); \
    Shizu_Environment_set(state, environment1, Shizu_String_create(state, #Name, strlen(#Name)), &value);

  Define(StartOfInput)
  Define(EndOfInput)
  Define(Error)
  Define(Name)
  Define(Integer)
  Define(LeftParenthesis)
  Define(RightParenthesis)
  Define(LeftSquareBracket)
  Define(RightSquareBracket)
  Define(Period)
  Define(Comma)
  Define(SingleLineComment)
  Define(EndOfLine)

#undef Define
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Environment* environment1 = getOrCreateEnvironment(state, environment, Shizu_String_create(state, "AstType", strlen("AstType")));
    Shizu_Value value;
  #define Define(Name) \
    Shizu_Value_setInteger32(&value, Compiler_AstType_##Name); \
    Shizu_Environment_set(state, environment1, Shizu_String_create(state, #Name, strlen(#Name)), &value);

    Define(Class)
    Define(Method)
    Define(Program)

  #undef Define
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }

  fprintf(stdout, "[Module : Compiler] loaded\n");
}

Shizu_Module_Export void
Shizu_ModuleLibrary_unload
  (
    Shizu_State2* state
  )
{
  fprintf(stdout, "[Module : Compiler] unloaded\n");
}

Shizu_Module_Export char const*
Shizu_ModuleLibrary_getName
  (
    Shizu_State1* state
  )
{
  static const char* NAME = "Shizu Module Compiler";
  return NAME;
}

Shizu_Module_Export void
Shizu_ModuleLibrary_update
  (
    Shizu_State2* state
  )
{ fprintf(stdout, "[Module : Compiler] update\n"); }
