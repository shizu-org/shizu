#include "Shizu/Runtime/Object.h"

// fprintf, stderr
#include <stdio.h>
// exit, EXIT_FAILURE
#include <stdlib.h>
#include "Shizu/Runtime/State2.h"

static void
Shizu_Error_emitMethodCallContext
  (
    Shizu_State2* state,
    FILE* f,
    Shizu_SourceLocationC sourceLocationC,
    Shizu_Object* callee,
    char const* methodName,
    size_t methodNameLength
  )
{
  // Cxx file.
  fprintf(f, "file: %s", sourceLocationC.file);
  
  // Cxx line.
  fprintf(f, ", ");
  fprintf(f, "line: %d", sourceLocationC.line);

  // Callee address.
  fprintf(f, ", ");
  fprintf(f, "callee: %p", callee);

  // Callee type address/name.
  Shizu_Type* calleeType = ((Shizu_Object*)callee)->type;
  char const* calleeTypeName; size_t calleeTypeNameLength;
  Shizu_Types_getTypeName(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), calleeType, &calleeTypeName, &calleeTypeNameLength);
  fprintf(f, ", ");
  fprintf(f, "callee type: %p/", calleeType);
  fwrite(calleeTypeName, 1, calleeTypeNameLength, f);

  // Method name.
  fprintf(f, ", ");
  fprintf(f, "callee method: ");
  fwrite(methodName, 1, methodNameLength, f);
}

Shizu_NoReturn() void
Shizu_Errors_raiseDispatchNotExists
  (
    Shizu_State2* state,
    Shizu_SourceLocationC sourceLocationC,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Error_emitMethodCallContext(state, stderr, sourceLocationC, target, methodName, methodNameLength);
  fprintf(stderr, ": dispatch not created\n");
  Shizu_State2_setStatus(state, Shizu_Status_DispatchNotExists);
  Shizu_State2_jump(state);
}

Shizu_NoReturn() void
Shizu_Errors_raiseMethodNotImplemented
  (
    Shizu_State2* state,
    Shizu_SourceLocationC sourceLocationC,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Error_emitMethodCallContext(state, stderr, sourceLocationC, target, methodName, methodNameLength);
  fprintf(stderr, ": call to undefined method\n");
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotImplemented);
  Shizu_State2_jump(state);
}

void
Shizu_Object_construct
  (
  Shizu_State2* state,
  Shizu_Object* self
  )
{
  self->type = Shizu_Object_getType(state);
}
