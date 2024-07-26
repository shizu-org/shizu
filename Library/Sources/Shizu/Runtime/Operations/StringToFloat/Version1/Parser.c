#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Operations/StringToFloat/Version1/Parser.h"

#include "Shizu/Runtime/Objects/String.h"

#define UpperCaseE ('E')
#define LowerCaseE ('e')
#define Plus ('+')
#define Minus ('-')
#define Zero ('0')
#define Nine ('9')
#define Period ('.')
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
    ParserState_CallbackContext* callbackContext;
    ParserState_CallbackFunction* callbackFunction;
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
  char const* integralStart = NULL;
  char const* integralEnd = NULL;
  char const* fractionalStart = NULL;
  char const* fractionalEnd = NULL;
  if (isDigit(state, self)) {
    integralStart = self->input.current;
    integralEnd = self->input.current;
    do {
      next(state, self);
    } while (isDigit(state, self));
    integralEnd = self->input.current;
    fractionalStart = self->input.current;
    fractionalEnd = self->input.current;
    if (Period == self->input.symbol) {
      next(state, self);
      fractionalStart = self->input.current;
      while (isDigit(state, self)) {
        next(state, self);
      }
      fractionalEnd = self->input.current;
    }
  } else if (Period == self->input.symbol) {
    integralStart = self->input.current;
    integralEnd = self->input.current;
    next(state, self);
    fractionalStart = self->input.current;
    fractionalEnd = self->input.current;
    if (!isDigit(state, self)) {
      syntaxError(state);
    }
    do {
      next(state, self);
    } while (isDigit(state, self));
    fractionalEnd = self->input.current;
  } else {
    syntaxError(state);
  }
  bool exponentSign = true;
  char const* exponentStart = self->input.current;
  char const* exponentEnd = self->input.current;
  if (UpperCaseE == self->input.symbol || LowerCaseE == self->input.symbol) {
    next(state, self);
    switch (self->input.symbol) {
      case Plus: {
        next(state, self);
      } break;
      case Minus: {
        next(state, self);
        sign = false;
      } break;
    };
    exponentStart = self->input.current;
    exponentEnd = self->input.current;
    if (!isDigit(state, self)) {
      syntaxError(state);
    }
    do {
      next(state, self);
    } while (isDigit(state, self));
    exponentEnd = self->input.current;
  }
  if (End != self->input.symbol) {
    syntaxError(state);
  }
  self->output.callbackFunction(state, self->output.callbackContext, sign, integralStart, integralEnd - integralStart, fractionalStart, fractionalEnd - fractionalStart,
                                                                     exponentSign, exponentStart, exponentEnd - exponentStart);
}

void
Shizu_Operations_StringToFloat_Version1_parse
  (
    Shizu_State2* state,
    Shizu_String* source,
    ParserState_CallbackContext* callbackContext,
    ParserState_CallbackFunction* callbackFunction
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
