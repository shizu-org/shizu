#if !defined(SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED

#include "Shizu/Runtime/Compiler/Object.h"

Shizu_declareType(Shizu_Runtime_Compiler_Token)

struct Shizu_Runtime_Compiler_Token_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Token {
  Shizu_Runtime_Compiler_Object _parent;
};

#endif // SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED
