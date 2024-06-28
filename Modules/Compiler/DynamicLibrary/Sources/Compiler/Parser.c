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

#include "Compiler/Parser.h"

#include "Compiler/Ast.h"
#include "Compiler/Scanner.h"

#include "idlib/byte_sequence.h"

static void
Compiler_Parser_visit
  (
    Shizu_State2* state,
    Compiler_Parser* self
  );

static void
Compiler_Parser_initializeDispatch
  (
    Shizu_State2* state,
    Compiler_Parser_Dispatch* self
  );

static void
Compiler_Parser_callImpl
  (
    Shizu_State2* state,
    Compiler_Parser* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

static Shizu_TypeDescriptor const Compiler_Parser_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Parser),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Parser_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Parser_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Compiler_Parser_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Compiler_Parser, Compiler_Object);

static void
Compiler_Parser_visit
  (
    Shizu_State2* state,
    Compiler_Parser* self
  )
{ 
  if (self->scanner) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->scanner);
  }
}

static void
Compiler_Parser_initializeDispatch
  (
    Shizu_State2* state,
    Compiler_Parser_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->call = (void (*)(Shizu_State2*, Shizu_Object*, uint8_t const*, size_t, Shizu_Value*, Shizu_Integer32, Shizu_Value*)) & Compiler_Parser_callImpl;
}

static void
Compiler_Parser_callImpl
  (
    Shizu_State2* state,
    Compiler_Parser* self,
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
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
        Shizu_State2_jump(state);
      }
      Compiler_Ast* ast = Compiler_Parser_run(state, self);
      Shizu_Value_setObject(returnValue, (Shizu_Object*)ast);
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("setInput") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "setInput", sizeof("setInput") - 1);
    if (!result) {
      if (1 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
        Shizu_State2_jump(state);
      }
      if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&arguments[0])->type,
                                   Shizu_String_getType(state))) {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
        Shizu_State2_jump(state);
      }
      Compiler_Parser_setInput(state, self, (Shizu_String*)Shizu_Value_getObject(&arguments[0]));
      Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
      return;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
  Shizu_State2_jump(state);
}

void
Compiler_Parser_construct
  (
    Shizu_State2* state,
    Compiler_Parser* self
  )
{
  Shizu_Type* TYPE = Compiler_Parser_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  self->scanner = Compiler_Scanner_create(state);
  ((Shizu_Object*)self)->type = TYPE;
}

Compiler_Parser*
Compiler_Parser_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Compiler_Parser_getType(state);
  Compiler_Parser* self = (Compiler_Parser*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Parser));
  Compiler_Parser_construct(state, self);
  return self;
}

static inline void
step
  (
    Shizu_State2* state,
    Compiler_Parser* self
  )
{ Compiler_Scanner_step(state, self->scanner); }

static inline Compiler_TokenType
getTokenType
  (
    Shizu_State2* state,
    Compiler_Parser* self
  )
{ return Compiler_Scanner_getTokenType(state, self->scanner); }

Compiler_Ast*
Compiler_Parser_run
  (
    Shizu_State2* state,
    Compiler_Parser* self
  )
{
  if (Compiler_TokenType_StartOfInput != getTokenType(state, self)) {
    Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
    Shizu_State2_jump(state);
  }
  step(state, self);
  Compiler_Ast* fileAst = Compiler_Ast_create(state, Compiler_AstType_File, Shizu_String_create(state, "", sizeof("") - 1));
  while (Compiler_TokenType_Error != getTokenType(state, self) && Compiler_TokenType_EndOfInput != getTokenType(state, self)) {
    step(state, self);
  }
  return fileAst;
}

void
Compiler_Parser_setInput
  (
    Shizu_State2* state,
    Compiler_Parser* self,
    Shizu_String* input
  )
{
  Compiler_Scanner_setInput(state, self->scanner, input);
}
