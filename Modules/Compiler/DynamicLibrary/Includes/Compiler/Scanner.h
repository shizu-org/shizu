#if !defined(COMPILER_SCANNER_H_INCLUDED)
#define COMPILER_SCANNER_H_INCLUDED

#include "Shizu/Runtime/Objects/ByteArray.h"
#include "Compiler/Object.h"
#include "Compiler/Token.h"

Shizu_declareType(Compiler_Scanner)

struct Compiler_Scanner_Dispatch {
  Compiler_Object_Dispatch _parent;
};

struct Compiler_Scanner {
  Compiler_Object _parent;
  Shizu_String* input;
  Compiler_TokenType tokenType;
  char const* start;
  char const* end;
  char const* current;
  Shizu_ByteArray* buffer;
};

void
Compiler_Scanner_construct
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  );

Compiler_Scanner*
Compiler_Scanner_create
  (
    Shizu_State2* state
  );

void
Compiler_Scanner_setInput
  (
    Shizu_State2* state,
    Compiler_Scanner* self,
    Shizu_String* input
  );

Shizu_String*
Compiler_Scanner_getInput
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  );

void
Compiler_Scanner_step
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  );

Compiler_TokenType
Compiler_Scanner_getTokenType
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  );

#endif // COMPILER_SCANNER_H_INCLUDED
