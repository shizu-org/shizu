#if !defined(SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED

#include "Shizu/Runtime/Objects/ByteArray.h"
#include "Compiler/Object.h"
#include "Compiler/Token.h"

Shizu_declareType(Shizu_Runtime_Compiler_Scanner)

struct Shizu_Runtime_Compiler_Scanner_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Scanner {
  Shizu_Runtime_Compiler_Object _parent;
  Shizu_String* input;
  Shizu_Runtime_Compiler_TokenType tokenType;
  char const* start;
  char const* end;
  char const* current;
  Shizu_ByteArray* buffer;
};

void
Shizu_Runtime_Compiler_Scanner_construct
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Scanner* self
  );

Shizu_Runtime_Compiler_Scanner*
Shizu_Runtime_Compiler_Scanner_create
  (
    Shizu_State* state
  );

void
Shizu_Runtime_Compiler_Scanner_setInput
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Scanner* self,
    Shizu_String* input
  );

Shizu_String*
Shizu_Runtime_Compiler_Scanner_getInput
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Scanner* self
  );

void
Shizu_Runtime_Compiler_Scanner_step
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Scanner* self
  );

#endif // SHIZU_RUNTIME_COMPILER_SCANNER_H_INCLUDED
