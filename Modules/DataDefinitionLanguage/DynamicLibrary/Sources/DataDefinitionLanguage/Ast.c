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

#include "idlib/byte_sequence.h"

Shizu_defineEnumerationType("MachineLanguage.AstType", AstType);

static void
Ast_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static void
Ast_callImpl
  (
    Shizu_State2* state,
    Ast* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

static void
Ast_visit
  (
    Shizu_State2* state,
    Ast* self
  );

static void
Ast_initializeDispatch
  (
    Shizu_State2* state,
    Ast_Dispatch* self
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
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Ast_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("DataDefinitionLanguage.Ast", Ast, Shizu_Object);

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
  if (Shizu_Value_isObject(&argumentValues[2])) {
    self->text = Shizu_Runtime_Extensions_getStringValue(state, &argumentValues[2]);
  } else if (Shizu_Value_isVoid(&argumentValues[2])) {
    self->text = NULL;
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  self->children = Shizu_Runtime_Extensions_createList(state);
  ((Shizu_Object*)self)->type = TYPE;
}

static void
Ast_callImpl
  (
    Shizu_State2* state,
    Ast* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  )
{
  if (numberOfMethodNameBytes == sizeof("getType") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "getType", sizeof("getType") - 1);
    if (!result) {
      if (0 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Shizu_Value_setInteger32(returnValue, self->type);
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("getText") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "getText", sizeof("getText") - 1);
    if (!result) {
      if (0 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      if (self->text) {
        Shizu_Value_setObject(returnValue, (Shizu_Object*)self->text);
      } else {
        Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
      }
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("getNumberOfChildren") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "getNumberOfChildren", sizeof("getNumberOfChildren") - 1);
    if (!result) {
      if (0 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Shizu_Value_setInteger32(returnValue, Shizu_List_getSize(state, self->children));
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("getChildAt") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "getChildAt", sizeof("getChildAt") - 1);
    if (!result) {
      if (1 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Shizu_Integer32 index = Shizu_Runtime_Extensions_getInteger32Value(state, &(arguments[0]));
      *returnValue = Shizu_List_getValue(state, self->children, index);
      return;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotFound);
  Shizu_State2_jump(state);
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

static void
Ast_initializeDispatch
  (
    Shizu_State2* state,
    Ast_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->call = (void (*)(Shizu_State2*, Shizu_Object*, uint8_t const*, size_t, Shizu_Value*, Shizu_Integer32, Shizu_Value*)) & Ast_callImpl;
}

Ast*
Ast_create
  (
    Shizu_State2* state,
    AstType type,
    Shizu_String* text
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer(), Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Ast_getType(state));
  Shizu_Value_setInteger32(&argumentValues[1], type);
  if (text) {
    Shizu_Value_setObject(&argumentValues[2], (Shizu_Object*)text);
  } else {
    Shizu_Value_setVoid(&argumentValues[2], Shizu_Void_Void);
  }
  Shizu_Operations_create(state, &returnValue, 3, &argumentValues[0]);
  return (Ast*)Shizu_Value_getObject(&returnValue);
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

Shizu_Integer32
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
    Shizu_Integer32 index
  )
{
  Shizu_Value value = Shizu_List_getValue(state, self->children, index);
  if (!Shizu_Value_isObject(&value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return (Ast*)Shizu_Value_getObject(&value);
}
