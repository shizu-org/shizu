#if !defined(MACHINELANGUAGE_SCANNER_H_INCLUDED)
#define MACHINELANGUAGE_SCANNER_H_INCLUDED

#include "Shizu/Runtime/Include.h"
#include "MachineLanguage/Token.h"

/// The
/// @code
/// class Scanner
/// @endcode
/// type.
/// Its constructor is
/// @code
/// Scanner.construct()
/// @endcode
Shizu_declareObjectType(Scanner);

struct Scanner_Dispatch {
  Shizu_Object_Dispatch _parent;
};

#define Symbol_StartOfInput (255-2)
#define Symbol_EndOfInput (255-1)
#define Symbol_Error (255-2)

struct Scanner {
  Shizu_Object _parent;
  Shizu_String* input;
  TokenType tokenType;
  struct {
    int symbol;
    char const* start;
    char const* end;
    char const* current;
  } reader;
  Shizu_ByteArray* buffer;
};

Scanner*
Scanner_create
  (
    Shizu_State2* state
  );

void
Scanner_setInput
  (
    Shizu_State2* state,
    Scanner* self,
    Shizu_String* input
  );

Shizu_String*
Scanner_getInput
  (
    Shizu_State2* state,
    Scanner* self
  );

void
Scanner_step
  (
    Shizu_State2* state,
    Scanner* self
  );

TokenType
Scanner_getTokenType
  (
    Shizu_State2* state,
    Scanner* self
  );

Shizu_String*
Scanner_getTokenText
  (
    Shizu_State2* state,
    Scanner* self
  );

#endif // MACHINELANGUAGE_SCANNER_H_INCLUDED
