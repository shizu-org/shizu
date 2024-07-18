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

#include "MachineLanguage/Parser.h"

#include "MachineLanguage/Ast.h"
#include "MachineLanguage/Scanner.h"

#include "idlib/byte_sequence.h"

static void
Parser_visit
  (
    Shizu_State2* state,
    Parser* self
  );

static void
Parser_initializeDispatch
  (
    Shizu_State2* state,
    Parser_Dispatch* self
  );

static void
Parser_callImpl
  (
    Shizu_State2* state,
    Parser* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

static void
Parser_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static inline void
step
  (
    Shizu_State2* state,
    Parser* self
  );

static inline TokenType
getTokenType
  (
    Shizu_State2* state,
    Parser* self
  );

static inline Shizu_String*
getTokenText
  (
    Shizu_State2* state,
    Parser* self
  );

static Shizu_ObjectTypeDescriptor const Parser_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Parser),
  .construct = &Parser_constructImpl,
  .visit = (Shizu_OnVisitCallback*)&Parser_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Parser_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Parser_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("MachineLanguage.Parser", Parser, Shizu_Object);

static void
Parser_visit
  (
    Shizu_State2* state,
    Parser* self
  )
{
  if (self->scanner) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->scanner);
  }
}

static void
Parser_initializeDispatch
  (
    Shizu_State2* state,
    Parser_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->call = (void (*)(Shizu_State2*, Shizu_Object*, uint8_t const*, size_t, Shizu_Value*, Shizu_Integer32, Shizu_Value*)) & Parser_callImpl;
}

static void
Parser_callImpl
  (
    Shizu_State2* state,
    Parser* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  )
{
  if (numberOfMethodNameBytes == sizeof("run") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "run", sizeof("run") - 1);
    if (!result) {
      if (numberOfArguments != 0) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Ast* ast = Parser_run(state, self);
      Shizu_Value_setObject(returnValue, (Shizu_Object*)ast);
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("setInput") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "setInput", sizeof("setInput") - 1);
    if (!result) {
      if (1 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&arguments[0])->type,
                                   Shizu_String_getType(state))) {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
        Shizu_State2_jump(state);
      }
      Parser_setInput(state, self, (Shizu_String*)Shizu_Value_getObject(&arguments[0]));
      Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
      return;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotFound);
  Shizu_State2_jump(state);
}

static void
Parser_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Parser_getType(state);
  Parser* self = (Parser*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->scanner = Scanner_create(state);
  ((Shizu_Object*)self)->type = TYPE;
}

Parser*
Parser_create
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void), };
  Shizu_Value_setType(&argumentValues[0], Parser_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Parser*)Shizu_Value_getObject(&returnValue);
}

static inline void
step
  (
    Shizu_State2* state,
    Parser* self
  )
{
  Scanner_step(state, self->scanner);
  while (TokenType_SingleLineComment == getTokenType(state, self) || TokenType_MultiLineComment == getTokenType(state, self)) {
    Scanner_step(state, self->scanner);
  }
}

static inline TokenType
getTokenType
  (
    Shizu_State2* state,
    Parser* self
  )
{ return Scanner_getTokenType(state, self->scanner); }

static inline Shizu_String*
getTokenText
  (
    Shizu_State2* state,
    Parser* self
  )
{ return Scanner_getTokenText(state, self->scanner); }

static Ast* parseClass(Shizu_State2* state, Parser* self) {
  Shizu_debugAssert(TokenType_ClassKeyword == getTokenType(state, self));
  step(state, self);
  if (TokenType_Name != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  Ast* classAst = Ast_create(state, AstType_Class, getTokenText(state, self));
  return classAst;
}

static Ast* parseParameter(Shizu_State2* state, Parser* self) {
  Ast* parameterAst = Ast_create(state, AstType_Parameter, NULL);
  // <name>
  if (TokenType_Name != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  Ast* parameterNameAst = Ast_create(state, AstType_ParameterName, getTokenText(state, self));
  Ast_append(state, parameterAst, parameterNameAst);
  step(state, self);
  // `:`
  if (TokenType_Colon != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  // <name>
  if (TokenType_Name != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  Ast* parameterTypeAst = Ast_create(state, AstType_ParameterType, getTokenText(state, self));
  Ast_append(state, parameterAst, parameterTypeAst);
  step(state, self);
  //
  return parameterAst;
}

static Ast* parseParameterList(Shizu_State2* state, Parser* self) {
  if (TokenType_LeftParenthesis != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  Ast* parameterListAst = Ast_create(state, AstType_ParameterList, NULL);
  if (TokenType_RightParenthesis != getTokenType(state, self)) {
    while (true) {
      Ast* parameterAst = parseParameter(state, self);
      if (TokenType_Comma != getTokenType(state, self)) {
        break;
      }
      step(state, self);
    }
  }
  if (TokenType_RightParenthesis != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  return parameterListAst;  
}

static Ast* parseProcedure(Shizu_State2* state, Parser* self) {
  Shizu_debugAssert(TokenType_ProcedureKeyword == getTokenType(state, self));
  step(state, self);
  if (TokenType_Name != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  Ast* procedureAst = Ast_create(state, AstType_Procedure, getTokenText(state, self));
  step(state, self);
  Ast* parameterListAst = parseParameterList(state, self);
  Ast_append(state, procedureAst, parameterListAst);
  // <return type>
  if (TokenType_Colon != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  if (TokenType_Name != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  Ast* returnType = Ast_create(state, AstType_ReturnType, getTokenText(state, self));
  Ast_append(state, procedureAst, returnType);
  step(state, self);
  // <procedure body>
  if (TokenType_LeftCurlyBracket != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  if (TokenType_RightCurlyBracket != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  Ast* procedureBodyAst = Ast_create(state, AstType_ProcedureBody, NULL);
  Ast_append(state, procedureAst, procedureBodyAst);
  return procedureAst;
}

Ast*
Parser_run
  (
    Shizu_State2* state,
    Parser* self
  )
{
  if (TokenType_StartOfInput != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  Ast* fileAst = Ast_create(state, AstType_File, Shizu_String_create(state, "", sizeof("") - 1));
  while (TokenType_Error != getTokenType(state, self) && TokenType_EndOfInput != getTokenType(state, self)) {
    switch (getTokenType(state, self)) {
      case TokenType_ClassKeyword: {
        Ast* classAst = parseClass(state, self);
        Ast_append(state, fileAst, classAst);
      } break;
      case TokenType_ProcedureKeyword: {
        Ast* procedureAst = parseProcedure(state, self);
        Ast_append(state, fileAst, procedureAst);
      } break;
      case TokenType_Name: {

      } break;
      default: {
        step(state, self);
      } break;
    };
  }
  return fileAst;
}

void
Parser_setInput
  (
    Shizu_State2* state,
    Parser* self,
    Shizu_String* input
  )
{
  Scanner_setInput(state, self->scanner, input);
}
