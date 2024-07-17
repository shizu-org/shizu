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

#include "MachineLanguage/Scanner.h"

#include "idlib/byte_sequence.h"

static void
scanKeyword
  (
    Shizu_State2* state2,
    TokenType* tokenType,
    char const* bytes,
    size_t numberOfBytes
  )
{
  typedef struct Keyword {
    char const* bytes;
    size_t numberOfBytes;
    TokenType tokenType;
  } Keyword;
  static const Keyword g_keywords[] = {
    { .bytes = "class", .numberOfBytes = sizeof("constructor") - 1, .tokenType = TokenType_ClassKeyword },
    { .bytes = "constructor", .numberOfBytes = sizeof("constructor") - 1, .tokenType = TokenType_ConstructorKeyword },
    { .bytes = "method", .numberOfBytes = sizeof("method") - 1, .tokenType = TokenType_MethodKeyword },
    { .bytes = "procedure", .numberOfBytes = sizeof("procedure") - 1, .tokenType = TokenType_ProcedureKeyword },
  };
  static const size_t g_numberOfKeywords = sizeof(g_keywords) / sizeof(Keyword);
  for (size_t i = 0, n = g_numberOfKeywords; i < n; ++i) {
    if (numberOfBytes == g_keywords[i].numberOfBytes) {
      int8_t result;
      idlib_byte_sequence_compare(&result, bytes, g_keywords[i].bytes, numberOfBytes);
      if (!result) {
        *tokenType = g_keywords[i].tokenType;
        return;
      }
    }
  }
  return; 
}

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

Shizu_defineObjectType("MachineLanguage.Scanner", Scanner, Shizu_Object);

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
  self->reader.start = Shizu_String_getBytes(state, self->input);
  self->reader.end = self->reader.start + Shizu_String_getNumberOfBytes(state, self->input);
  self->reader.current = self->reader.start;
  self->reader.symbol = Symbol_StartOfInput;
  self->tokenType = TokenType_StartOfInput;
  ((Shizu_Object*)self)->type = TYPE;
}

Scanner*
Scanner_create
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Scanner_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Scanner*)Shizu_Value_getObject(&returnValue);
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
  self->reader.start = Shizu_String_getBytes(state, self->input);
  self->reader.end = self->reader.start + Shizu_String_getNumberOfBytes(state, self->input);
  self->reader.current = self->reader.start;
  self->reader.symbol = Symbol_StartOfInput;
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

static inline void
next
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  if (self->reader.end == self->reader.current) {
      self->reader.symbol = Symbol_EndOfInput;
  } else {
    self->reader.symbol = *self->reader.current;
    self->reader.current++;
  }
}

static inline void
write
  (
    Shizu_State2* state,
    Scanner* self,
    int symbol
  )
{
  if (self->reader.symbol == Symbol_StartOfInput ||
      self->reader.symbol == Symbol_EndOfInput ||
      self->reader.symbol == Symbol_Error) {
    Shizu_State2_setStatus(state, Shizu_Status_LexicalError);
    Shizu_State2_jump(state);
  }
  uint8_t x = (uint8_t)symbol;
  Shizu_ByteArray_appendRawBytes(state, self->buffer, &x, 1);
}

static inline void
save
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  write(state, self, self->reader.symbol);
}

static inline void
writeAndNext
  (
    Shizu_State2* state,
    Scanner* self,
    int symbol
  )
{
  write(state, self, symbol);
  next(state, self);
}

static inline void
saveAndNext
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  save(state, self);
  next(state, self);
}

static inline bool
isUnderscore
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return ('_' == self->reader.symbol);
}

static inline bool
isAlpha
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return ('a' <= self->reader.symbol && self->reader.symbol <= 'z')
      || ('A' <= self->reader.symbol && self->reader.symbol <= 'Z');
}

static inline bool
isDigit
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return ('0' <= self->reader.symbol && self->reader.symbol <= '9');
}

static inline bool
isWhitespace
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return ' '  == self->reader.symbol
      || '\t' == self->reader.symbol;
}

static inline bool
isNewline
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return '\n' == self->reader.symbol
      || '\r' == self->reader.symbol;
}

static inline void
scanDoubleQuotedString
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  Shizu_ByteArray_clear(state, self->buffer);
  next(state, self);
  bool lastWasSlash = false;
  while (true) {
    if (Symbol_EndOfInput == self->reader.symbol) {
      // Unclosed string literal.
      self->tokenType = TokenType_Error;
      return;
    }
    if ('"' == self->reader.symbol) {
      next(state, self);
      break;
    }
    if (isNewline(state, self)) {
      // Unclosed string literal.
      self->tokenType = TokenType_Error;
      return;
    }
    if (lastWasSlash) {
      switch (self->reader.symbol) {
        case '\\': {
          lastWasSlash = false;
          writeAndNext(state, self, '\\');
          continue;
        } break;
        case 'n': {
          lastWasSlash = false;
          writeAndNext(state, self, '\n');
          continue;
        } break;
        case 'r': {
          lastWasSlash = false;
          writeAndNext(state, self, '\r');
          continue;
        } break;
        case '"': {
          lastWasSlash = false;
          writeAndNext(state, self, '"');
          continue;
        } break;
        default: {
          // Unknown escape sequence.
          self->tokenType = TokenType_Error;
          return;
        } break;
      }
    } else if ('\\' == self->reader.symbol) {
      lastWasSlash = true;
      next(state, self);
      continue;
    } else {
      saveAndNext(state, self);
      continue;
    }
  }
  self->tokenType = TokenType_String;
}

static void
scanExponent
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  if ('e' == self->reader.symbol || 'E' == self->reader.symbol) {
    // exponent : ('e'|'E') ('+'|'-')? digit+
    saveAndNext(state, self);
    if ('+' == self->reader.symbol || '-' == self->reader.symbol) {
      saveAndNext(state, self);
    }
    if (!isDigit(state, self)) {
      self->tokenType = TokenType_Error;
      return;
    }
    do {
      saveAndNext(state, self);
    } while (isDigit(state, self));
    self->tokenType = TokenType_Real;
  }
}

static void
skipNewLineInComment
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  if (isNewline(state, self)) {
    next(state, self);
    int old = self->reader.symbol;
    if (isNewline(state, self) && old != self->reader.symbol) {
      next(state, self);
    }
  }
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
    if (isWhitespace(state, self)) {
      do {
        next(state, self);
      } while (isWhitespace(state, self));
      run = true;
    }
    // Parse newlines.
    if (isNewline(state, self)) {
      do {
        next(state, self);
        int old = self->reader.symbol;
        if (isNewline(state, self) && old != self->reader.symbol) {
          next(state, self);
        }
      } while (isNewline(state, self));
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
  if (Symbol_StartOfInput == self->reader.symbol) {
    next(state, self);
  }
  // Skip whitespaces and newlines.
  skipWhiteSpacesAndNewLines(state, self);
  if (Symbol_EndOfInput == self->reader.symbol) {
    self->tokenType = TokenType_EndOfInput;
    return;
  }
  switch (self->reader.symbol) {
    case '"': {
      scanDoubleQuotedString(state, self);
      return;
    } break;
    case '(': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_LeftParenthesis;
      return;
    } break;
    case ')': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_RightParenthesis;
      return;
    } break;
    case '{': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_LeftCurlyBracket;
      return;
   } break;
    case '}': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_RightCurlyBracket;
      return;
    } break;
    case ':': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_Colon;
      return;
    } break;
    case '0': case '1': case '2':
    case '3': case '4': case '5':
    case '6': case '7': case '8':
    case '9': {
      Shizu_ByteArray_clear(state, self->buffer);
      do {
        saveAndNext(state, self);
      } while(isDigit(state, self));
      self->tokenType = TokenType_Integer;
      if ('.' == self->reader.symbol) {
        saveAndNext(state, self);
        while (isDigit(state, self)) {
          saveAndNext(state, self);
        }
        self->tokenType = TokenType_Real;
        scanExponent(state, self);
      }
    } break;
    case '+':
    case '-': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      if (isDigit(state, self)) {
        do {
          saveAndNext(state, self);
        } while (isDigit(state, self));
        self->tokenType = TokenType_Integer;
        if ('.' == self->reader.symbol) {
          saveAndNext(state, self);
          while (isDigit(state, self)) {
            saveAndNext(state, self);
          }
          self->tokenType = TokenType_Real;
          scanExponent(state, self);
        }
      } else if ('.' == self->reader.symbol) {
          saveAndNext(state, self);
          if (!isDigit(state, self)) {
            self->tokenType = TokenType_Error;
            return;
          }
          self->tokenType = TokenType_Real;
          while (isDigit(state, self)) {
            saveAndNext(state, self);
          }
          scanExponent(state, self);
      } else {
        self->tokenType = TokenType_Error;
        return;
      }
    } break;
    case '.': {
      saveAndNext(state, self);
      self->tokenType = TokenType_Period;
      if (isDigit(state, self)) {
        do {
          saveAndNext(state, self);
        } while (isDigit(state, self));
      }
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      return;
    } break;
    case ',': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_Comma;
      return;
    } break;
    case ';': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_Semicolon;
      return;
    } break;
    case '=': {
      Shizu_ByteArray_clear(state, self->buffer);
      saveAndNext(state, self);
      self->tokenType = TokenType_Equal;
      return;
    } break;
    case '/': {
      Shizu_ByteArray_clear(state, self->buffer);
      next(state, self);
      if ('/' == self->reader.symbol) {
        next(state, self);
        while (Symbol_EndOfInput != self->reader.symbol && !isNewline(state, self)) {
          saveAndNext(state, self);
        }
        self->tokenType = TokenType_SingleLineComment;
        return;
      } else if ('*' == self->reader.symbol) {
        next(state, self);
        while (true) {
          if ('*' == self->reader.symbol) {
            next(state, self);
            if ('/' == self->reader.symbol) {
              next(state, self);
              break;
            } else {
              saveAndNext(state, self);
            }
          } else if (isNewline(state, self)) {
            skipNewLineInComment(state, self);
          } else if (Symbol_EndOfInput == self->reader.symbol) {
            /* Unclosed multi-line comment. */
            self->tokenType = TokenType_Error;
            return;
          } else  {
            saveAndNext(state, self);
          }
        }
        self->tokenType = TokenType_MultiLineComment;
        return;
      } else {
        self->tokenType = TokenType_Error;
        return;
      }
    } break;
    default: {
    } break;
  };
  // name
  Shizu_ByteArray_clear(state, self->buffer);
  if (isAlpha(state, self) || isUnderscore(state, self)) {
    saveAndNext(state, self);
    while (isAlpha(state, self) || isDigit(state, self) || isUnderscore(state, self)) {
      saveAndNext(state, self);
    }
    self->tokenType = TokenType_Name;
    scanKeyword(state, &self->tokenType, Shizu_ByteArray_getRawBytes(state, self->buffer), Shizu_ByteArray_getNumberOfRawBytes(state, self->buffer));
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

Shizu_String*
Scanner_getTokenText
  (
    Shizu_State2* state,
    Scanner* self
  )
{
  return Shizu_String_create(state, Shizu_ByteArray_getRawBytes(state, self->buffer),
                                    Shizu_ByteArray_getNumberOfRawBytes(state, self->buffer));
}
