#if !defined(SHIZU_RUNTIME_COMPILER_AST_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_AST_H_INCLUDED

#include "Shizu/Runtime/Compiler/Object.h"

Shizu_declareType(Shizu_Runtime_Compiler_Ast)

struct Shizu_Runtime_Compiler_Ast_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Ast {
  Shizu_Runtime_Compiler_Object _parent;
};

#endif // SHIZU_RUNTIME_COMPILER_AST_H_INCLUDED
