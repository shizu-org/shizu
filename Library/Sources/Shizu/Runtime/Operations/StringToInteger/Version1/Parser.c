#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/StringToInteger/Version1/Parser.h"

#include "Shizu/Runtime/Objects/String.h"

#define Plus ('+')
#define Minus ('-')
#define Zero ('0')
#define Nine ('9')
#define Start (255-2)
#define End (255-1)

typedef struct ParserState {
  struct {
    char const* current;
    char const* start;
    char const* end;
    int symbol;
  } input;
  struct {
    Shizu_Operations_StringToInteger_Parser_CallbackContext* callbackContext;
    Shizu_Operations_StringToInteger_Parser_CallbackFunction* callbackFunction;
  } output;
} ParserState;

Shizu_Cxx_NoReturn() static void
syntaxError
  (
    Shizu_State2* state
  )
{
  Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
}

static void
next
  (
    Shizu_State2* state,
    ParserState* self
  )
{
  if (self->input.symbol == End) {
    return;
  } else if (self->input.symbol != Start) {
    self->input.current++;
  }
  if (self->input.current == self->input.end) {
    self->input.symbol = End;
  } else {
    self->input.symbol = *self->input.current;
  }
}

static bool
isDigit
  (
    Shizu_State2* state,
    ParserState* self
  )
{
  return Zero <= self->input.symbol
      && self->input.symbol <= Nine;
}

static void
run
  (
    Shizu_State2* state,
    ParserState* self
  )
{
  if (Start != self->input.symbol) {
    syntaxError(state);
  }
  next(state, self);
  bool sign = true;
  switch (self->input.symbol) {
    case Plus: {
      next(state, self);
    } break;
    case Minus: {
      next(state, self);
      sign = false;
    } break;
  };
  char const* digitsStart = NULL;
  char const* digitsEnd = NULL;
  if (isDigit(state, self)) {
    digitsStart = self->input.current;
    digitsEnd = self->input.current;
    do {
      next(state, self);
    } while (isDigit(state, self));
    digitsEnd = self->input.current;
  } else {
    syntaxError(state);
  }
  if (End != self->input.symbol) {
    syntaxError(state);
  }
  self->output.callbackFunction(state, self->output.callbackContext, sign, digitsStart, digitsEnd - digitsStart);
}

void
Shizu_Operations_StringToInteger_Parser_parse
  (
    Shizu_State2* state,
    Shizu_String* source,
    Shizu_Operations_StringToInteger_Parser_CallbackContext* callbackContext,
    Shizu_Operations_StringToInteger_Parser_CallbackFunction* callbackFunction
  )
{
  ParserState parserState;
  parserState.input.start = Shizu_String_getBytes(state, source);
  parserState.input.end = parserState.input.start + Shizu_String_getNumberOfBytes(state, source);
  parserState.input.current = parserState.input.start;
  parserState.input.symbol = Start;
  parserState.output.callbackContext = callbackContext;
  parserState.output.callbackFunction = callbackFunction;
  run(state, &parserState);
}
