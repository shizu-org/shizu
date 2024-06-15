#if !defined(SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED

#include "Compiler/Object.h"

typedef enum Shizu_Runtime_Compiler_TokenType Shizu_Runtime_Compiler_TokenType;

enum Shizu_Runtime_Compiler_TokenType  {
  Shizu_Runtime_Compiler_TokenType_StartOfInput,
  Shizu_Runtime_Compiler_TokenType_EndOfInput,
  Shizu_Runtime_Compiler_TokenType_ClassKeyword,
  Shizu_Runtime_Compiler_TokenType_MethodKeyword,
  Shizu_Runtime_Compiler_TokenType_ConstructorKeyword,
  Shizu_Runtime_Compiler_TokenType_IfKeyword,
  Shizu_Runtime_Compiler_TokenType_ElseKeyword,
  Shizu_Runtime_Compiler_TokenType_Name,
  Shizu_Runtime_Compiler_TokenType_LeftParenthesis,
  Shizu_Runtime_Compiler_TokenType_RightParenthesis,
  Shizu_Runtime_Compiler_TokenType_LeftSquareBracket,
  Shizu_Runtime_Compiler_TokenType_RightSquareBracket,
  Shizu_Runtime_Compiler_TokenType_Period,
  Shizu_Runtime_Compiler_TokenType_Comma,
  Shizu_Runtime_Compiler_TokenType_Plus,
  Shizu_Runtime_Compiler_TokenType_Minus,
  Shizu_Runtime_Compiler_TokenType_Integer,
  Shizu_Runtime_Compiler_TokenType_Real,
  Shizu_Runtime_Compiler_TokenType_EndOfLine,
};

Shizu_declareType(Shizu_Runtime_Compiler_Token)

struct Shizu_Runtime_Compiler_Token_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Token {
  Shizu_Runtime_Compiler_Object _parent;
  Shizu_Runtime_Compiler_TokenType type;
  Shizu_String* text;
};

void
Shizu_Runtime_Compiler_Token_construct
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Token* self,
    Shizu_Runtime_Compiler_TokenType type,
    Shizu_String* text
  );

Shizu_Runtime_Compiler_Token*
Shizu_Runtime_Compiler_Token_create
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_TokenType type,
    Shizu_String* text
  );

#endif // SHIZU_RUNTIME_COMPILER_TOKEN_H_INCLUDED
