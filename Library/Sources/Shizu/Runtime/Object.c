#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Object.h"

// fprintf, stderr
#include <stdio.h>
// exit, EXIT_FAILURE
#include <stdlib.h>
#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Type.private.h"

static void
Shizu_Error_emitMethodCallContext
  (
    Shizu_State2* state,
    FILE* f,
    Shizu_Cxx_SourceLocation cxxSourceLocation,
    Shizu_Object* callee,
    char const* methodName,
    size_t methodNameLength
  )
{
  // Cxx file.
  fprintf(f, "file: %s", cxxSourceLocation.file);

  // Cxx line.
  fprintf(f, ", ");
  fprintf(f, "line: %d", cxxSourceLocation.line);

  // Callee address.
  fprintf(f, ", ");
  fprintf(f, "callee: %p", callee);

  // Callee type address/name.
  Shizu_Type* calleeType = ((Shizu_Object*)callee)->type;
  uint8_t const* calleeTypeName; Shizu_Integer32 calleeTypeNameLength;
  Shizu_Types_getTypeName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), calleeType, &calleeTypeName, &calleeTypeNameLength);
  fprintf(f, ", ");
  fprintf(f, "callee type: %p/", calleeType);
  fwrite(calleeTypeName, 1, calleeTypeNameLength, f);

  // Method name.
  fprintf(f, ", ");
  fprintf(f, "callee method: ");
  fwrite(methodName, 1, methodNameLength, f);
}

Shizu_Cxx_NoReturn() void
Shizu_Errors_raiseDispatchNotExists
  (
    Shizu_State2* state,
    Shizu_Cxx_SourceLocation cxxSourceLocation,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Error_emitMethodCallContext(state, stderr, cxxSourceLocation, target, methodName, methodNameLength);
  fprintf(stderr, ": dispatch not created\n");
  Shizu_State2_setStatus(state, Shizu_Status_DispatchNotExists);
  Shizu_State2_jump(state);
}

Shizu_Cxx_NoReturn() void
Shizu_Errors_raiseMethodNotImplemented
  (
    Shizu_State2* state,
    Shizu_Cxx_SourceLocation cxxSourceLocation,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Error_emitMethodCallContext(state, stderr, cxxSourceLocation, target, methodName, methodNameLength);
  fprintf(stderr, ": call to undefined method\n");
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotImplemented);
  Shizu_State2_jump(state);
}

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  );

static Shizu_Integer32
Shizu_Object_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

static Shizu_Boolean
Shizu_Object_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Value const* other
  );

static void
Shizu_Object_initializeDispatch
  (
    Shizu_State1* state1,
    Shizu_Object_Dispatch* self
  );

static void
Shizu_Object_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Shizu_Object_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Object),
  .construct = NULL,
  .visit = NULL,
  .finalize = NULL,
  .dispatchSize = sizeof(Shizu_Object_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Shizu_Object_initializeDispatch,
  .dispatchUninitialize = NULL,
};

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  )
{/*Intentionally empty.*/}

static Shizu_Integer32
Shizu_Object_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_Object* self
  )
{
  return (Shizu_Integer32)(intptr_t)self;
}

static Shizu_Boolean
Shizu_Object_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Value const* other
  )
{
  if (Shizu_Value_Tag_Object != other->tag) {
    return Shizu_Boolean_False;
  }
  return self == other->objectValue;
}

static void
Shizu_Object_initializeDispatch
  (
    Shizu_State1* state1,
    Shizu_Object_Dispatch* self
  )
{
  self->getHashValue = &Shizu_Object_getHashValueImpl;
  self->isEqualTo = &Shizu_Object_isEqualToImpl;
}

Shizu_Type*
Shizu_Object_getType
  (
    Shizu_State2* state
  )
{
  Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state),
                                               Shizu_State2_getTypes(state),
                                               "Shizu_Object",
                                               sizeof("Shizu_Object") - 1);
  if (!type) {
    type = Shizu_Types_createObjectType(Shizu_State2_getState1(state),
                                        Shizu_State2_getTypes(state),
                                        "Shizu_Object",
                                        sizeof("Shizu_Object") - 1,
                                        NULL,
                                        NULL,
                                        &Shizu_Object_typeDestroyed,
                                        &Shizu_Object_Type);
  }
  return type;
}

static void
Shizu_Object_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Value_isObject(&argumentValues[0])) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Object* self = (Shizu_Object*)Shizu_Value_getObject(&argumentValues[0]);
  self->type = Shizu_Object_getType(state);
}

void
Shizu_Object_construct
  (
  Shizu_State2* state,
  Shizu_Object* self
  )
{
  Shizu_Value returnValue;
  Shizu_Value_setVoid(&returnValue, Shizu_Void_Void);
  Shizu_Value argumentValue;
  Shizu_Value_setObject(&argumentValue, self);
  Shizu_Object_constructImpl(state, &returnValue, 1, &argumentValue);
}

void
Shizu_Object_call
  (
    Shizu_State2* state,
    Shizu_Object* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  )
{ Shizu_VirtualCall(Shizu_Object, call, self, methodNameBytes, numberOfMethodNameBytes, returnValue, numberOfArguments, arguments); }

Shizu_Integer32
Shizu_Object_getHashValue
  (
    Shizu_State2* state,
    Shizu_Object* self
  )
{ Shizu_VirtualCallWithReturn(Shizu_Object, getHashValue, self); }

Shizu_Boolean
Shizu_Object_isEqualTo
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Value const* other
  )
{ Shizu_VirtualCallWithReturn(Shizu_Object, isEqualTo, self, other); }

Shizu_Type*
Shizu_Object_getObjectType
  (
    Shizu_State2* state,
    Shizu_Object* self
  )
{
  Shizu_Cxx_Debug_assert(NULL != state);
  Shizu_Cxx_Debug_assert(NULL != self);
  Shizu_Cxx_Debug_assert(NULL != self->type);
  return self->type;
}

Shizu_Object_Dispatch*
Shizu_Object_getObjectDispatch
  (
    Shizu_State2* state,
    Shizu_Object* self
  )
{
  Shizu_Type* type = Shizu_Object_getObjectType(state, self);
  return type->objectType.dispatch;
}
