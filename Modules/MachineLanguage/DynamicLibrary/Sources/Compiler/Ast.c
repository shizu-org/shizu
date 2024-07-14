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

// Utility function to create a "Shizu.List".
static Shizu_List* createList(Shizu_State2* state) {
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Shizu_List_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_List*)Shizu_Value_getObject(&returnValue);
}

Shizu_defineEnumerationType(Compiler_AstType);

static void
Compiler_Ast_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static void
Compiler_Ast_visit
  (
    Shizu_State2* state,
    Compiler_Ast* self
  );

static Shizu_ObjectTypeDescriptor const Compiler_Ast_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Ast),
  .construct = &Compiler_Ast_constructImpl,
  .visit = (Shizu_OnVisitCallback*)&Compiler_Ast_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Ast_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Compiler_Ast, Compiler_Object);

static void
Compiler_Ast_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Compiler_Ast_getType(state);
  Compiler_Ast* self = (Compiler_Ast*)Shizu_Value_getObject(&argumentValues[0]);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  self->type = Shizu_Value_getInteger32(&argumentValues[1]);
  if (Shizu_Value_isObject(&argumentValues[2])) {
    self->text = Shizu_Runtime_Extensions_getStringValue(state, &argumentValues[2]);
  } else if (Shizu_Value_isVoid(&argumentValues[2])) {
    self->text = NULL;
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  self->children = createList(state);
  ((Shizu_Object*)self)->type = TYPE;
}

static void
Compiler_Ast_visit
  (
    Shizu_State2* state,
    Compiler_Ast* self
  )
{
  if (self->children) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->children);
  }
  if (self->text) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->text);
  }
}

Compiler_Ast*
Compiler_Ast_create
  (
    Shizu_State2* state,
    Compiler_AstType type,
    Shizu_String* text
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer(), Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Compiler_Ast_getType(state));
  Shizu_Value_setInteger32(&argumentValues[1], type);
  if (text) {
    Shizu_Value_setObject(&argumentValues[2], (Shizu_Object*)text);
  } else {
    Shizu_Value_setVoid(&argumentValues[2], Shizu_Void_Void);
  }
  Shizu_Operations_create(state, &returnValue, 3, &argumentValues[0]);
  return (Compiler_Ast*)Shizu_Value_getObject(&returnValue);
}
