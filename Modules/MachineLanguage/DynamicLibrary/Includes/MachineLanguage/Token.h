#if !defined(MACHINELANGUAGE_TOKEN_H_INCLUDED)
#define MACHINELANGUAGE_TOKEN_H_INCLUDED

#include "Shizu/Runtime/Include.h"

Shizu_declareEnumerationType(TokenType);

enum TokenType  {
  TokenType_StartOfInput,
  TokenType_EndOfInput,
  TokenType_Error,
  TokenType_SingleLineComment,
  TokenType_MultiLineComment,
  // `procedure`
  TokenType_ProcedureKeyword,
  // `class`
  TokenType_ClassKeyword,
  // `method`
  TokenType_MethodKeyword,
  // `constructor`
  TokenType_ConstructorKeyword,
  // `if`
  TokenType_IfKeyword,
  // `else`
  TokenType_ElseKeyword,
  // <name>
  TokenType_Name,
  // `(`
  TokenType_LeftParenthesis,
  // `)`
  TokenType_RightParenthesis,
  // `:`
  TokenType_Colon,
  // `{` 
  TokenType_LeftCurlyBracket,
  // `}` 
  TokenType_RightCurlyBracket,
  TokenType_Period,
  TokenType_Comma,
#if 0
  TokenType_Plus,
  TokenType_Minus,
  TokenType_Star,
  TokenType_Slash,
#endif
  TokenType_Integer,
  TokenType_String,
  TokenType_Real,
  TokenType_EndOfLine,
  TokenType_Semicolon,
  TokenType_Equal,
};

Shizu_declareObjectType(Token)

struct Token_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Token {
  Shizu_Object _parent;
  TokenType type;
  Shizu_String* text;
};

void
Token_construct
  (
    Shizu_State2* state,
    Token* self,
    TokenType type,
    Shizu_String* text
  );

Token*
Token_create
  (
    Shizu_State2* state,
    TokenType type,
    Shizu_String* text
  );

#endif // MACHINELANGUAGE_TOKEN_H_INCLUDED
