#include "Shizu/Runtime/Include.h"

// fprintf, stdio
#include <stdio.h>

// strlen
#include <string.h>

#include "DataDefinitionLanguage/Ast.h"
#include "DataDefinitionLanguage/Token.h"
#include "DataDefinitionLanguage/Parser.h"
#include "DataDefinitionLanguage/Scanner.h"

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
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, (Shizu_Object*)Scanner_create(state));
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
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(returnValue, (Shizu_Object*)Parser_create(state));
}

Shizu_Module_Export void
Shizu_ModuleLibrary_load
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Dl* dl = NULL;
  Shizu_JumpTarget jumpTarget;

  Shizu_Environment* environment = NULL;
  environment = Shizu_Runtime_Extensions_getOrCreateEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "DataDefinitionLanguage", strlen("DataDefinitionLanguage")));

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
    Shizu_Environment* environment1 = Shizu_Runtime_Extensions_getOrCreateEnvironment(state, environment, Shizu_String_create(state, "TokenType", strlen("TokenType")));
    Shizu_Value value;
#define Define(Name) \
    Shizu_Value_setInteger32(&value, TokenType_##Name); \
    Shizu_Environment_set(state, environment1, Shizu_String_create(state, #Name, strlen(#Name)), &value);

  Define(StartOfInput)
  Define(EndOfInput)
  Define(Error)
  Define(Name)
  Define(Integer)
  Define(Real)
  Define(String)
  Define(LeftCurlyBracket)
  Define(RightCurlyBracket)
  Define(LeftSquareBracket)
  Define(RightSquareBracket)
  Define(Colon)
  Define(Comma)
  Define(SingleLineComment)

#undef Define
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }

  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Environment* environment1 = Shizu_Runtime_Extensions_getOrCreateEnvironment(state, environment, Shizu_String_create(state, "AstType", strlen("AstType")));
    Shizu_Value value;
  #define Define(Name) \
    Shizu_Value_setInteger32(&value, AstType_##Name); \
    Shizu_Environment_set(state, environment1, Shizu_String_create(state, #Name, strlen(#Name)), &value);

    Define(File)
    Define(Integer)
    Define(List)
    Define(Map)
    Define(MapElement)
    Define(Name)
    Define(Real)
    Define(String)

  #undef Define
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }

  fprintf(stdout, "[Module : DataDefinitionLanguage] loaded\n");
}

Shizu_Module_Export void
Shizu_ModuleLibrary_unload
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  fprintf(stdout, "[Module : DataDefinitionLanguage] unloaded\n");
}

Shizu_Module_Export char const*
Shizu_ModuleLibrary_getName
  (
    Shizu_State1* state
  )
{
  static const char* NAME = "Shizu Module DataDefinitionLanguage";
  return NAME;
}

Shizu_Module_Export void
Shizu_ModuleLibrary_update
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{ fprintf(stdout, "[Module : DataDefinitionLanguage] update\n"); }
