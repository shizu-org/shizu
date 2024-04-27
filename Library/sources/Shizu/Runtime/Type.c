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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Type.h"

#include "Shizu/Runtime/Type.private.h"

bool
Shizu_State_isSubTypeOf
  (
    Shizu_State* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  )
{
  Shizu_Type const* z = x;
  do {
    if (z == y) {
      break;
    }
    z = z->parentType;
  } while (z);
  return NULL != z;
}

bool
Shizu_Type_isTrueSubTypeOf
  (
    Shizu_State* self,
    Shizu_Type const* x,
    Shizu_Type const* y
  )
{
  Shizu_Type const* z = x;
  do {
    if (z->parentType == y) {
      break;
    }
    z = z->parentType;
  } while (z);
  return NULL != z;
}
