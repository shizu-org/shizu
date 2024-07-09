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

#include "DataDefinitionLanguage/Scanner.h"

#include "idlib/byte_sequence.h"

static void
Scanner_visit
  (
    Shizu_State2* state,
    Scanner* self
  );

static void
Scanner_initializeDispatch
  (
    Shizu_State2* state,
    Scanner_Dispatch* self
  );

static void
Scanner_callImpl
  (
    Shizu_State2* state,
    Scanner* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

static void
Scanner_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Scanner_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Scanner),
  .construct = &Scanner_constructImpl,
  .visit = (Shizu_OnVisitCallback*)&Scanner_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Scanner_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Scanner_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Scanner, Shizu_Object);

static void
Scanner_visit
  (
    Shizu_State2* state,
    Scanner* self
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
Scanner_initializeDispatch
  (
    Shizu_State2* state,
    Scanner_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->call = (void (*)(Shizu_State2*, Shizu_Object*, uint8_t const*, size_t, Shizu_Value*, Shizu_Integer32, Shizu_Value*)) & Scanner_callImpl;
}

static void
Scanner_callImpl
  (
    Shizu_State2* state,
    Scanner* self,
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
      Scanner_step(state, self);
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
      Scanner_setInput(state, self, (Shizu_String*)Shizu_Value_getObject(&arguments[0]));
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
      Shizu_Value_setInteger32(returnValue, Scanner_getTokenType(state, self));
      return;
    }
  }
  Shizu_State2_setStatus(state, Shizu_Status_MethodNotFound);
  Shizu_State2_jump(state);
}

static void
Scanner_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  Shizu_Type* TYPE = Scanner_getType(state);
  Scanner* self = (Scanner*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->buffer = Shizu_ByteArray_create(state);
  self->input = Shizu_String_create(state, "", sizeof("") - 1);
  self->start = Shizu_String_getBytes(state, self->input);
  self->end = self->start + Shizu_String_getNumberOfBytes(state, self->input);
  self->current = self->start;
  self->tokenType = TokenType_StartOfInput;
  ((Shizu_Object*)self)->type = TYPE;
}

void
Scanner_construct
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  Shizu_Type* TYPE = Scanner_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->buffer = Shizu_ByteArray_create(state);
  self->input = Shizu_String_create(state, "", sizeof("") - 1);
  self->start = Shizu_String_getBytes(state, self->input);
  self->end = self->start + Shizu_String_getNumberOfBytes(state, self->input);
  self->current = self->start;
  self->tokenType = TokenType_StartOfInput;
  ((Shizu_Object*)self)->type = TYPE;
}

Scanner*
Scanner_create
  (
    Shizu_State2* state
  )
{
  Shizu_Type* TYPE = Scanner_getType(state);
  Scanner* self = (Scanner*)Shizu_Gc_allocateObject(state, sizeof(Scanner));
  Scanner_construct(state, self);
  return self;
}

void
Scanner_setInput
  (
    Shizu_State2* state,
    Scanner* self,
    Shizu_String* input
  )
{
  self->input = input;
  self->start = Shizu_String_getBytes(state, self->input);
  self->end = self->start + Shizu_String_getNumberOfBytes(state, self->input);
  self->current = self->start;
  self->tokenType = TokenType_StartOfInput;
}

Shizu_String*
Scanner_getInput
  (
    Shizu_State2* state,
    Scanner* self
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
    Scanner* self
  )
{
  self->current++;
  bool lastWasSlash = false;
  while (true) {
    if (self->current == self->end) {
      // Unclosed string literal.
      self->tokenType = TokenType_Error;
      return;
    }
    if ('"' == *self->current) {
      self->current++;
      break;
    }
    if (isNewline(self->end, self->current)) {
      // Unclosed string literal.
      self->tokenType = TokenType_Error;
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
          self->tokenType = TokenType_Error;
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
  self->tokenType = TokenType_String;
}

static void
skipWhiteSpacesAndNewLines
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  bool run;
  do {
    run = false;
    // Consume whitespaces.
    if (isWhitespace(self->end, self->current)) {
      do {
        self->current++;
      } while (isWhitespace(self->end, self->current));
      run = true;
    }
    // Parse newlines.
    if (isNewline(self->end, self->current)) {
      do {
        self->current++;
        char old = *self->current;
        if (isNewline(self->end, self->current) && old != *self->current) {
          self->current++;
        }
        // @todo Increment line counter.
      } while (isNewline(self->end, self->current));
      run = true;
    }
  } while (run);
}

void
Scanner_step
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  if (self->tokenType == TokenType_EndOfInput) {
    return;
  }
  // Skip whitespaces and newlines.
  skipWhiteSpacesAndNewLines(state, self);
  if (self->current == self->end) {
    self->tokenType = TokenType_EndOfInput;
    return;
  }
  switch (*self->current) {
    case '"': {
      scanDoubleQuotedString(state, self);
      return;
    } break;
    case '{': {
      self->tokenType = TokenType_LeftCurlyBracket;
      self->current++;
      return;
    } break;
    case '}': {
      self->tokenType = TokenType_RightCurlyBracket;
      self->current++;
      return;
    } break;
    case '[': {
      self->tokenType = TokenType_LeftSquareBracket;
      self->current++;
      return;
   } break;
    case ']': {
      self->tokenType = TokenType_RightSquareBracket;
      self->current++;
      return;
    } break;
    case '.': {
      self->current++;
      if (!isDigit(self->end, self->current)) {
        self->tokenType = TokenType_Error;
        return;
      }
      do {
        self->current++;
      } while (isDigit(self->end, self->current));
      self->tokenType = TokenType_Real;
      return;
    } break;
    case ',': {
      self->tokenType = TokenType_Comma;
      self->current++;
      return;
    } break;
    case ':': {
      self->tokenType = TokenType_Colon;
      self->current++;
      return;
    } break;
    case '/': {
      self->current++;
      if (self->current == self->end || '/' == *self->current) {
        self->tokenType = TokenType_Error;
        return;
      }
      self->current++;
      while (self->end != self->current && !isNewline(self->end, self->current)) {
        self->current++;
      }
      self->tokenType = TokenType_SingleLineComment;
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
   self->tokenType = TokenType_Name;
  } else if (isAlpha(self->end, self->current)) {
    do {
      self->current++;
    } while (isUnderscore(self->end, self->current) || isDigit(self->end, self->current) || isAlpha(self->end, self->current));
    self->tokenType = TokenType_Name;
  } else if (isDigit(self->end, self->current)) {
    do {
      self->current++;
    } while (isDigit(self->end, self->current));
    self->tokenType = TokenType_Integer;
    if (self->end != self->current && '.' == *self->current) {
      self->current++;
      if (isDigit(self->end, self->current)) {
        do {
          self->current++;
        } while (isDigit(self->end, self->current));
      }
      if (self->end != self->current && ('e' == *self->current || 'E' == *self->current)) {
        self->current++;
        if (self->end != self->current && ('+' == *self->current || '-' == *self->current)) {
          self->current++;
        }
        if (!isDigit(self->end, self->current)) {
          self->tokenType = TokenType_Error;
          return;
        }
        do {
          self->current++;
        } while (isDigit(self->end, self->current));
      }
      self->tokenType = TokenType_Real;
    }
  } else {
    Shizu_State2_setStatus(state, Shizu_Status_LexicalError);
    Shizu_State2_jump(state);
  }
}

TokenType
Scanner_getTokenType
  (
    Shizu_State2* state,
    Scanner* self
  )
{ return self->tokenType; }
