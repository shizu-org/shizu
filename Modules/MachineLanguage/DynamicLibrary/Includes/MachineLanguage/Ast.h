#if !defined(MACHINELANGUAGE_AST_H_INCLUDED)
#define MACHINELANGUAGE_AST_H_INCLUDED

#include "Shizu/Runtime/Include.h"

Shizu_declareEnumerationType(AstType);

enum AstType {
  // text = <class name>, children = [ <extends>, <body> ]
  AstType_Class,
  AstType_File,
  AstType_Method,
  // text = <procedure name>, children = [ <parameter list>, <return type>, <procedure body> ]
  AstType_Procedure,
  AstType_ProcedureBody,
  AstType_Program,
  AstType_Constructor,
  // text = null, children = [ <parameter>* ]
  AstType_ParameterList,
  // text = null, children = [ <parameter name>, <parameter type> ]
  AstType_Parameter,
  AstType_ParameterName,
  // text = <type name>, children = []
  AstType_ParameterType,
  // text = <type name>, children = []
  AstType_ReturnType,
};

/// The
/// @code
/// module class Ast
/// @endcode
/// type.
/// Its constructor is
/// @code
/// Ast.construct(type : Integer32, text : String | Void)
/// @endcode
Shizu_declareObjectType(Ast)

struct Ast_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Ast {
  Shizu_Object _parent;
  AstType type;
  Shizu_String* text;
  Shizu_List* children;
};

Ast*
Ast_create
  (
    Shizu_State2* state,
    AstType type,
    Shizu_String* text
  );

void
Ast_append
  (
    Shizu_State2* state,
    Ast* self,
    Ast* other
  );

Shizu_Integer32
Ast_getNumberOfChildren
  (
    Shizu_State2* state,
    Ast* self
  );

Ast*
Ast_getChild
  (
    Shizu_State2* state,
    Ast* self,
    Shizu_Integer32 index
  );

#endif // MACHINELANGUAGE_AST_H_INCLUDED
