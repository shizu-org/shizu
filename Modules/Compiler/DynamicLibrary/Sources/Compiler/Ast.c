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

#include "Compiler/Ast.h"

Shizu_defineEnumerationType(Compiler_AstType);

static void
Compiler_Ast_visit
  (
    Shizu_State2* state,
    Compiler_Ast* self
  )
{
  if (self->text) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->text);
  }
}

static Shizu_ObjectTypeDescriptor const Compiler_Ast_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Ast),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Ast_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Ast_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Compiler_Ast, Compiler_Object);

void
Compiler_Ast_construct
  (
    Shizu_State2* state,
    Compiler_Ast* self,
    Compiler_AstType type,
    Shizu_String* text
  )
{
  Shizu_Type* TYPE = Compiler_Ast_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  self->type = type;
  self->text = text;
  ((Shizu_Object*)self)->type = TYPE;
}

Compiler_Ast*
Compiler_Ast_create
  (
    Shizu_State2* state,
    Compiler_AstType type,
    Shizu_String* text
  )
{
  Shizu_Type* TYPE = Compiler_Ast_getType(state);
  Compiler_Ast* self = (Compiler_Ast*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Ast));
  Compiler_Ast_construct(state, self, type, text);
  return self;
}
