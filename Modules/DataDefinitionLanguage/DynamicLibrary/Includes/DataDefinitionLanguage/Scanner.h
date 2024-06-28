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

#if !defined(DATADEFINITIONLANGUAGE_SCANNER_H_INCLUDED)
#define DATADEFINITIONLANGUAGE_SCANNER_H_INCLUDED

#include "Shizu/Runtime/Include.h"
#include "DataDefinitionLanguage/Token.h"

Shizu_declareType(Scanner);

struct Scanner_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Scanner {
  Shizu_Object _parent;
  Shizu_String* input;
  TokenType tokenType;
  char const* start;
  char const* end;
  char const* current;
  Shizu_ByteArray* buffer;
};

void
Scanner_construct
  (
    Shizu_State2* state,
    Scanner* self
  );

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

#endif // DATADEFINITIONLANGUAGE_SCANNER_H_INCLUDED
