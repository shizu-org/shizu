#if !defined(SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED

#include "Shizu/Runtime/Compiler/Object.h"

Shizu_declareType(Shizu_Runtime_Compiler_Scanner)

struct Shizu_Runtime_Compiler_Scanner_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Scanner {
  Shizu_Runtime_Compiler_Object _parent;
};

void
Shizu_Runtime_Compiler_Scanner_construct
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Scanner* self
  );

#endif // SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED
