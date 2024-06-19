#if !defined(COMPILER_TOKEN_H_INCLUDED)
#define COMPILER_TOKEN_H_INCLUDED

#include "Compiler/Object.h"

Shizu_declareEnumerationType(Compiler_TokenType);

enum Compiler_TokenType  {
  Compiler_TokenType_StartOfInput,
  Compiler_TokenType_EndOfInput,
  Compiler_TokenType_Error,
  Compiler_TokenType_SingleLineComment,
  // `procedure`
  Compiler_TokenType_ProcedureKeyword,
  // `class`
  Compiler_TokenType_ClassKeyword,
  // `method`
  Compiler_TokenType_MethodKeyword,
  // `constructor`
  Compiler_TokenType_ConstructorKeyword,
  // `if`
  Compiler_TokenType_IfKeyword,
  // `else`
  Compiler_TokenType_ElseKeyword,
  Compiler_TokenType_Name,
  Compiler_TokenType_LeftParenthesis,
  Compiler_TokenType_RightParenthesis,
  Compiler_TokenType_LeftSquareBracket,
  Compiler_TokenType_RightSquareBracket,
  Compiler_TokenType_Period,
  Compiler_TokenType_Comma,
  Compiler_TokenType_Plus,
  Compiler_TokenType_Minus,
  Compiler_TokenType_Star,
  Compiler_TokenType_Slash,
  Compiler_TokenType_Integer,
  Compiler_TokenType_String,
  Compiler_TokenType_Real,
  Compiler_TokenType_EndOfLine,
};

Shizu_declareType(Compiler_Token)

struct Compiler_Token_Dispatch {
  Compiler_Object_Dispatch _parent;
};

struct Compiler_Token {
  Compiler_Object _parent;
  Compiler_TokenType type;
  Shizu_String* text;
};

void
Compiler_Token_construct
  (
    Shizu_State2* state,
    Compiler_Token* self,
    Compiler_TokenType type,
    Shizu_String* text
  );

Compiler_Token*
Compiler_Token_create
  (
    Shizu_State2* state,
    Compiler_TokenType type,
    Shizu_String* text
  );

#endif // COMPILER_TOKEN_H_INCLUDED
