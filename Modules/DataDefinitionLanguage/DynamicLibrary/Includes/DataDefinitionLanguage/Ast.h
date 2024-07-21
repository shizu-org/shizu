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
  AstType_Name,
  AstType_MapElement,
};

/// The
/// @code
/// module class Ast
/// @endcode
/// type.
/// It constructor is
/// @code
/// Ast.construct(type : Integer32, text : String | Void)
/// @endcode
/// The following methods are provided
/// @code
/// Ast.getType() : Integer32
/// Ast.getText() : String | Void
/// Ast.getNumberOfChildren() : Integer32
/// Ast.getChildAt(index : Integer32) : Ast
/// @endcode
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

Shizu_Integer32
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
    Shizu_Integer32 index
  );

#endif // DATADEFINITIONLANGUAGE_AST_H_INCLUDED
