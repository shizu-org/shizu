/*
  Shizu Runtime
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

#include "Compiler/Scanner.h"

#include "idlib/byte_sequence.h"

static void
Compiler_Scanner_visit
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  );

static void
Compiler_Scanner_initializeDispatch
  (
    Shizu_State2* state,
    Compiler_Scanner_Dispatch* self
  );

static void
Compiler_Scanner_callImpl
  (
    Shizu_State2* state,
    Compiler_Scanner* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

static Shizu_TypeDescriptor const Compiler_Scanner_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Scanner),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Scanner_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Scanner_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Compiler_Scanner_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Compiler_Scanner, Compiler_Object);

static void
Compiler_Scanner_visit
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{
  if (self->buffer) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->buffer);
  }
  if (self->input) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->input);
  }
}

static void
Compiler_Scanner_initializeDispatch
  (
    Shizu_State2* state,
    Compiler_Scanner_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->call = (void (*)(Shizu_State2*, Shizu_Object*, uint8_t const*, size_t, Shizu_Value*, Shizu_Integer32, Shizu_Value*)) & Compiler_Scanner_callImpl;
}

static void
Compiler_Scanner_callImpl
  (
    Shizu_State2* state,
    Compiler_Scanner* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  )
{ 
  if (numberOfMethodNameBytes == sizeof("step") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "step", sizeof("step") - 1);
    if (!result) {
      if (0 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Compiler_Scanner_step(state, self);
      Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("setInput") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "setInput", sizeof("setInput") - 1);
    if (!result) {
      if (1 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&arguments[0])->type, Shizu_String_getType(state))) {
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
        Shizu_State2_jump(state);
      }
      Compiler_Scanner_setInput(state, self, (Shizu_String*)Shizu_Value_getObject(&arguments[0]));
      Shizu_Value_setVoid(returnValue, Shizu_Void_Void);
      return;
    }
  }
  if (numberOfMethodNameBytes == sizeof("getTokenType") - 1) {
    int8_t result;
    idlib_byte_sequence_compare(&result, methodNameBytes, "getTokenType", sizeof("getTokenType") - 1);
    if (!result) {
      if (0 != numberOfArguments) {
        Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
        Shizu_State2_jump(state);
      }
      Shizu_Value_setInteger32(returnValue, Compiler_Scanner_getTokenType(state, self));
      return;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotFound);
  Shizu_State2_jump(state);
}

void
Compiler_Scanner_construct
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{
  Shizu_Type* TYPE = Compiler_Scanner_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  self->buffer = Shizu_ByteArray_create(state);
  self->input = Shizu_String_create(state, "", sizeof("") - 1);
  self->start = Shizu_String_getBytes(state, self->input);
  self->end = self->start + Shizu_String_getNumberOfBytes(state, self->input);
  self->current = self->start;
  self->tokenType = Compiler_TokenType_StartOfInput;
  ((Shizu_Object*)self)->type = TYPE;
}

Compiler_Scanner*
Compiler_Scanner_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Compiler_Scanner_getType(state);
  Compiler_Scanner* self = (Compiler_Scanner*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Scanner));
  Compiler_Scanner_construct(state, self);
  return self;
}

void
Compiler_Scanner_setInput
  (
    Shizu_State2* state,
    Compiler_Scanner* self,
    Shizu_String* input
  )
{
  self->input = input;
  self->start = Shizu_String_getBytes(state, self->input);
  self->end = self->start + Shizu_String_getNumberOfBytes(state, self->input);
  self->current = self->start;
  self->tokenType = Compiler_TokenType_StartOfInput;
}

Shizu_String*
Compiler_Scanner_getInput
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{
  return self->input;
}

static inline bool
isUnderscore
  (
    char const* end,
    char const* x
  )
{
  if (end == x) {
    return false;
  }
  return ('_' == *x);
}

static inline bool
isAlpha
  (
    char const* end,
    char const *x
  )
{
  if (end == x) {
    return false;
  }
  return ('a' <= *x && *x <= 'z')
      || ('A' <= *x && *x <= 'Z');
}

static inline bool
isDigit
  (
    char const* end,
    char const* x
  )
{
  if (end == x) {
    return false;
  }
  return ('0' <= *x && *x <= '9');
}

static inline bool
isWhitespace
  (
    char const* end,
    char const* x
  )
{
  if (end == x) {
    return false;
  }
  return ' ' == *x
      || '\t' == *x;
}

static inline bool
isNewline
  (
    char const* end,
    char const* x
  )
{
  if (end == x) {
    return false;
  }
  return '\n' == *x
      || '\r' == *x;
}

static inline void
scanDoubleQuotedString
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{
  self->current++;
  bool lastWasSlash = false;
  while (true) {
    if (self->current == self->end) {
      // Unclosed string literal.
      self->tokenType = Compiler_TokenType_Error;
      return;
    }
    if ('"' == *self->current) {
      self->current++;
      break;
    }
    if (isNewline(self->end, self->current)) {
      // Unclosed string literal.
      self->tokenType = Compiler_TokenType_Error;
      return;
    }
    if (lastWasSlash) {
      switch (*self->current) {
        case '\\': {
          lastWasSlash = false;
          self->current++;
          continue;
        } break;
        case 'n': {
          lastWasSlash = false;
          self->current++;
          continue;
        } break;
        case 'r':{
          lastWasSlash = false;
          self->current++;
          continue;
        } break;
        case '"': {
          lastWasSlash = false;
          self->current++;
          continue;
        } break;
        default: {
          // Unknown escape sequence.
          self->tokenType = Compiler_TokenType_Error;
          return;
        } break;
      }
    } else if ('\\' == *self->current) {
      lastWasSlash = true;
      self->current++;
      continue;
    } else {
      self->current++;
      continue;
    }
  }
  self->tokenType = Compiler_TokenType_String;
}

void
Compiler_Scanner_step
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{
  if (self->tokenType == Compiler_TokenType_EndOfInput) {
    return;
  }
  // Consume whitespaces.
  while (isWhitespace(self->end, self->current)) {
    do {
      self->current++;
    } while (isWhitespace(self->end, self->current));
  }
  // Parse newlines.
  if (isNewline(self->end, self->current)) {
    do {
      self->current++;
      char old = *self->current;
      if (isNewline(self->end, self->current) && old != *self->current) {
        self->current++; 
      }
    } while (isNewline(self->end, self->current));
    self->tokenType = Compiler_TokenType_EndOfLine;
    return;
  }
  
  if (self->current == self->end) {
    self->tokenType = Compiler_TokenType_EndOfInput;
    return;
  }
  switch (*self->current) {
    case '"': {
      scanDoubleQuotedString(state, self);
      return;
    } break;
    case '(': {
      self->tokenType = Compiler_TokenType_LeftParenthesis;
      self->current++;
      return;
    } break;
    case ')': {
      self->tokenType = Compiler_TokenType_RightParenthesis;
      self->current++;
    } break;
    case '{': {
      self->tokenType = Compiler_TokenType_LeftSquareBracket;
      self->current++;
      return;
   } break;
    case '}': {
      self->tokenType = Compiler_TokenType_RightSquareBracket;
      self->current++;
      return;
    } break;
    case '.': {
      self->tokenType = Compiler_TokenType_Period;
      self->current++;
      if (isDigit(self->end, self->current)) {
        do {
          self->current++;
        } while (isDigit(self->end, self->current));
        self->tokenType = Compiler_TokenType_Real;
      }
      return;
    } break;
    case ',': {
      self->tokenType = Compiler_TokenType_Comma;
      self->current++;
      return;
    } break;
    case ';': {
      self->tokenType = Compiler_TokenType_Semicolon;
      self->current++;
      return;
    } break;
    case '+': {
      self->tokenType = Compiler_TokenType_Plus;
      self->current++;
      return;
    } break;
    case '-': {
      self->tokenType = Compiler_TokenType_Minus;
      self->current++;
      return;
    } break;
    case '*': {
      self->tokenType = Compiler_TokenType_Star;
      self->current++;
      return;
    } break;
    case '=': {
      self->tokenType = Compiler_TokenType_Equal;
      self->current++;
      return;
    } break;
    case '/': {
      self->tokenType = Compiler_TokenType_Slash;
      self->current++;
      if (self->current != self->end && '/' == *self->current) {
        self->current++;
        while (self->end != self->current && !isNewline(self->end, self->current)) {
          self->current++;
        }
        self->tokenType = Compiler_TokenType_SingleLineComment;
      }
      return;
    } break;
    default: {
    } break;
  };
  // name
  if (isUnderscore(self->end, self->current)) {
    do {
      self->current++;
   } while (isUnderscore(self->end, self->current));
   if (isAlpha(self->end, self->current)) {
     do {
      self->current++;
     } while (isUnderscore(self->end, self->current) || isDigit(self->end, self->current) || isAlpha(self->end, self->current));
   }
   self->tokenType = Compiler_TokenType_Name;
  } else if (isAlpha(self->end, self->current)) {
    do {
      self->current++;
    } while (isUnderscore(self->end, self->current) || isDigit(self->end, self->current) || isAlpha(self->end, self->current));
    self->tokenType = Compiler_TokenType_Name;
  } else if (isDigit(self->end, self->current)) {
    do {
      self->current;
    } while (isDigit(self->end, self->current));
    self->tokenType = Compiler_TokenType_Integer;
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_LexicalError);
    Shizu_State2_jump(state);
  }
}

Compiler_TokenType
Compiler_Scanner_getTokenType
  (
    Shizu_State2* state,
    Compiler_Scanner* self
  )
{ return self->tokenType; }
