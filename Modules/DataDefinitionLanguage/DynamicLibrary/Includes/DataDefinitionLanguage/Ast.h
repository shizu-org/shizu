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

#if !defined(DATADEFINITIONLANGUAGE_AST_H_INCLUDED)
#define DATADEFINITIONLANGUAGE_AST_H_INCLUDED

#include "Shizu/Runtime/Include.h"

Shizu_declareEnumerationType(AstType);

enum AstType {
  AstType_Real,
  AstType_String,
  AstType_Integer,
  AstType_File,
  AstType_Map,
  AstType_List,
};

Shizu_declareObjectType(Ast);

struct Ast_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Ast {
  Shizu_Object _parent;
  AstType type;
  Shizu_String* text;
  Shizu_List* children;
};

void
Ast_construct
  (
    Shizu_State2* state,
    Ast* self,
    AstType type,
    Shizu_String* text
  );

Ast*
Ast_create
  (
    Shizu_State2* state,
    AstType type,
    Shizu_String* text
  );

void
Ast_append
  (
    Shizu_State2* state,
    Ast* self,
    Ast* other
  );

size_t
Ast_getNumberOfChildren
  (
    Shizu_State2* state,
    Ast* self
  );

Ast*
Ast_getChild
  (
    Shizu_State2* state,
    Ast* self,
    size_t index
  );

#endif // DATADEFINITIONLANGUAGE_AST_H_INCLUDED
