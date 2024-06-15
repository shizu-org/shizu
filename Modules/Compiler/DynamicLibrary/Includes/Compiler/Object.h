#if !defined(SHIZU_RUNTIME_COMPILER_OBJECT_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_OBJECT_H_INCLUDED

#include "Shizu/Runtime/Type.h"
#include "Shizu/Runtime/Gc.h"
#include "Shizu/Runtime/Object.h"

Shizu_declareType(Shizu_Runtime_Compiler_Object)

struct Shizu_Runtime_Compiler_Object_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Object {
  Shizu_Object _parent;
};

void
Shizu_Runtime_Compiler_Object_construct
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Object* self
  );

#endif // SHIZU_RUNTIME_COMPILER_OBJECT_H_INCLUDED
