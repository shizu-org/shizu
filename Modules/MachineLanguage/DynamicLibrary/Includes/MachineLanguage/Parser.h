#if !defined(MACHINELANGUAGE_PARSER_H_INCLUDED)
#define MACHINELANGUAGE_PARSER_H_INCLUDED

#include "Shizu/Runtime/Include.h"
typedef struct Ast Ast;
typedef struct Scanner Scanner;

/// The
/// @code
/// class Parser
/// @endcode
/// type.
/// Its constructor is
/// @code
/// Parser.construct()
/// @endcode
Shizu_declareObjectType(Parser);

struct Parser_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Parser {
  Shizu_Object _parent;
  Scanner* scanner;
};

Parser*
Parser_create
  (
    Shizu_State2* state
  );

Ast*
Parser_run
  (
    Shizu_State2* state,
    Parser* self
  );

void
Parser_setInput
  (
    Shizu_State2* state,
    Parser* self,
    Shizu_String* input
  );

#endif // MACHINELANGUAGE_PARSER_H_INCLUDED
