#include "Shizu/Runtime/Object.h"

// fprintf, stderr
#include <stdio.h>
// exit, EXIT_FAILURE
#include <stdlib.h>
#include "Shizu/Runtime/State.h"

Shizu_NoReturn() void
Shizu_Errors_raiseDispatchNotExists
  (
    Shizu_State* state,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Type* targetType = ((Shizu_Object*)target)->type;
  char const* p; size_t n;
  Shizu_Types_getTypeName(Shizu_State_getState1(state), Shizu_State_getTypes(state), targetType, &p, &n);
  fprintf(stderr, "file: %s, line: %d, target: %p, target type: %p", __FILE__, __LINE__, target, targetType);
  fprintf(stderr, ": fatal error (unreachable code reached): dispatch not created\n");
  Shizu_State_setStatus(state, Shizu_Status_DispatchNotExists);
  Shizu_State_jump(state);
}

Shizu_NoReturn() void
Shizu_Errors_raiseMethodNotImplemented
  (
    Shizu_State* state,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  )
{
  Shizu_Type* targetType = ((Shizu_Object*)target)->type;
  char const* p; size_t n;
  Shizu_Types_getTypeName(Shizu_State_getState1(state), Shizu_State_getTypes(state), targetType, &p, &n);
  fprintf(stderr, "file: %s, line: %d, object: %p, type: %p", __FILE__, __LINE__, target, targetType);
  fprintf(stderr, ": fatal error (unreachable code reached): call to undefined method\n");
  Shizu_State_setStatus(state, Shizu_Status_MethodNotImplemented);
  Shizu_State_jump(state);
}

void
Shizu_Object_construct
  (
  Shizu_State* state,
  Shizu_Object* self
  )
{
  self->type = Shizu_Object_getType(state);
}
