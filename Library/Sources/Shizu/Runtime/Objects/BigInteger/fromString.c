/*
  Shizu
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Objects/BigInteger/fromString.h"

#include "Shizu/Runtime/Objects/ByteArray.h"

#define Symbol_StartOfInput (255-2)
#define Symbol_EndOfInput (255-1)
#define Symbol_Error (255-0)

typedef struct Parser {
  uint32_t symbol;
  uint8_t const* current;
  uint8_t const* start;
  uint8_t const* end;

  Shizu_Boolean sign;
  Shizu_ByteArray* integral;
} Parser;

static inline Shizu_Boolean
isStartOfInput
  (
    Shizu_State2* state,
    Parser* self
  )
{
  return Symbol_StartOfInput == self->symbol;
}

static inline Shizu_Boolean
isEndOfInput
  (
    Shizu_State2* state,
    Parser* self
  )
{
  return Symbol_EndOfInput == self->symbol;
}

static inline Shizu_Boolean
isSign
  (
    Shizu_State2* state,
    Parser* self
  )
{
  return '+' == self->symbol || '-' == self->symbol;
}

static inline Shizu_Boolean
isDigit
  (
    Shizu_State2* state,
    Parser* self
  )
{
  return '0' <= self->symbol
      && self->symbol <= '9';
}

static inline Shizu_Boolean
isPeriod
  (
    Shizu_State2* state,
    Parser* self
  )
{
  return '.' == self->symbol;
}

static inline void
next
  (
    Shizu_State2* state,
    Parser* self
  )
{
  if (self->symbol == Symbol_EndOfInput || self->symbol == Symbol_Error) {
    return;
  } else if (self->symbol != Symbol_StartOfInput) {
    self->current++;
  }
  if (self->current == self->end) {
    self->symbol = Symbol_EndOfInput;
  } else {
    self->symbol = *self->current;
  }
}

static inline void
syntacticalError
  (
    Shizu_State2* state,
    Parser* self
  )
{
  Shizu_State2_setStatus(state, Shizu_Status_SyntacticalError);
  Shizu_State2_jump(state);
}

static inline void
Parser_run
  (
    Shizu_State2* state,
    Parser* self
  )
{
  if (!isStartOfInput(state, self)) {
    syntacticalError(state, self);
  }
  next(state, self);
  if (isSign(state, self)) {
    Shizu_debugAssert(Shizu_Boolean_True == self->sign);
    if ('-' == self->symbol) {
      self->sign = Shizu_Boolean_False;
    }
    next(state, self);
  }
  if (!isDigit(state, self)) {
    syntacticalError(state, self);
  }
  do {
    uint8_t x = (uint8_t)(self->symbol - (uint32_t)'0');
    Shizu_ByteArray_appendRawBytes(state, self->integral, &x, sizeof(uint8_t));
    next(state, self);
  } while (isDigit(state, self));
  if (!isEndOfInput(state, self)) {
    syntacticalError(state, self);
  }
}

static inline void
Parser_setInput
  (
    Shizu_State2* state,
    Parser* self,
    Shizu_String* input
  )
{
  self->current = (uint8_t const*)Shizu_String_getBytes(state, input);
  self->start = self->current;
  self->end = self->current + Shizu_String_getNumberOfBytes(state, input);
  self->symbol = Symbol_StartOfInput;

  self->sign = Shizu_Boolean_True;
  Shizu_ByteArray_clear(state, self->integral);
}

static inline void
Parser_initialize
  (
    Shizu_State2* state,
    Parser* self,
    Shizu_String* input
  )
{
  self->current = (uint8_t const*)Shizu_String_getBytes(state, input);
  self->start = self->current;
  self->end = self->current + Shizu_String_getNumberOfBytes(state, input);
  self->symbol = Symbol_StartOfInput;

  self->sign = Shizu_Boolean_True;
  self->integral = Shizu_ByteArray_create(state);
}

static inline void
Parser_uninitialize
  (
    Shizu_State2* state,
    Parser* self
  )
{/*Intentionally empty.*/}

void
Shizu_BigInteger_fromString
  (
    Shizu_State2* state,
    Shizu_String* v,
    Shizu_Boolean* sign,
    uint8_t** p,
    uint32_t* n
  )
{
  Parser parser;
  Parser_initialize(state, &parser, v);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Parser_run(state, &parser);
    uint8_t const* p1 = (uint8_t const*)Shizu_ByteArray_getRawBytes(state, parser.integral);
    size_t n0 = Shizu_ByteArray_getNumberOfRawBytes(state, parser.integral);
    uint8_t* p0 = Shizu_State1_allocate(Shizu_State2_getState1(state), n0);
    if (!p0) {
      Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
      Shizu_State2_jump(state);
    }
    memcpy(p0, p1, n0);
    *p = p0;
    *n = n0;
    *sign = parser.sign;
    Shizu_State2_popJumpTarget(state);
  } else {
    Parser_uninitialize(state, &parser);
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }
  Parser_uninitialize(state, &parser);
}
