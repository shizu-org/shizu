#if !defined(COMPILER_AST_H_INCLUDED)
#define COMPILER_AST_H_INCLUDED

#include "Compiler/Object.h"

Shizu_declareEnumerationType(Compiler_AstType);

enum Compiler_AstType {
  Compiler_AstType_ClassDef,
  Compiler_AstType_MethodDef,
  Compiler_AstType_ConstructorDef,
};

Shizu_declareType(Compiler_Ast)

struct Compiler_Ast_Dispatch {
  Compiler_Object_Dispatch _parent;
};

struct Compiler_Ast {
  Compiler_Object _parent;
  Compiler_AstType type;
  Shizu_String* text;
};

void
Compiler_Ast_construct
  (
    Shizu_State* state,
    Compiler_Ast* self,
    Compiler_AstType type,
    Shizu_String* text
  );

Compiler_Ast*
Compiler_Ast_create
  (
    Shizu_State* state,
    Compiler_AstType type,
    Shizu_String* text
  );

#endif // COMPILER_AST_H_INCLUDED
