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

#include "DataDefinitionLanguage/Ast.h"

// Utility function to create a "Shizu.List".
static Shizu_List* createList(Shizu_State2* state) {
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Shizu_List_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_List*)Shizu_Value_getObject(&returnValue);
}

Shizu_defineEnumerationType(AstType);

static void
Ast_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static void
Ast_visit
  (
    Shizu_State2* state,
    Ast* self
  );

static Shizu_ObjectTypeDescriptor const Ast_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Ast),
  .construct = &Ast_constructImpl,
  .visit = (Shizu_OnVisitCallback*)&Ast_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Ast_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Ast, Shizu_Object);

static void
Ast_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Ast_getType(state);
  Ast* self = (Ast*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->type = Shizu_Value_getInteger32(&argumentValues[1]);
  self->text = (Shizu_String*)Shizu_Value_getObject(&argumentValues[2]);
  self->children = createList(state);
  ((Shizu_Object*)self)->type = TYPE;
}

static void
Ast_visit
  (
    Shizu_State2* state,
    Ast* self
  )
{
  if (self->children) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->children);
  }
  if (self->text) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->text);
  }
}

void
Ast_construct
  (
    Shizu_State2* state,
    Ast* self,
    AstType type,
    Shizu_String* text
  )
{
  Shizu_Type* TYPE = Ast_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->type = type;
  self->text = text;
  self->children = createList(state);
  ((Shizu_Object*)self)->type = TYPE;
}

Ast*
Ast_create
  (
    Shizu_State2* state,
    AstType type,
    Shizu_String* text
  )
{
  Shizu_Type* TYPE = Ast_getType(state);
  Ast* self = (Ast*)Shizu_Gc_allocateObject(state, sizeof(Ast));
  Ast_construct(state, self, type, text);
  return self;
}

void
Ast_append
  (
    Shizu_State2* state,
    Ast* self,
    Ast* other
  )
{
  Shizu_List_appendObject(state, self->children, (Shizu_Object*)other);
}

size_t
Ast_getNumberOfChildren
  (
    Shizu_State2* state,
    Ast* self
  )
{
  return Shizu_List_getSize(state, self->children);
}

Ast*
Ast_getChild
  (
    Shizu_State2* state,
    Ast* self,
    size_t index
  )
{
  Shizu_Value value = Shizu_List_getValue(state, self->children, index);
  if (!Shizu_Value_isObject(&value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return (Ast*)Shizu_Value_getObject(&value);
}
