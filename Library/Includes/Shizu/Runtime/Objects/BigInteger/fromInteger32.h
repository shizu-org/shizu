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

#if !defined(SHIZU_OBJECTS_BIGINTEGER_FROMINTEGER32_H_INCLUDED)
#define SHIZU_OBJECTS_BIGINTEGER_FROMINTEGER32_H_INCLUDED

#include "Shizu/Runtime/Value.h"

void
Shizu_BigInteger_fromInteger32
  (
    Shizu_State2* state,
    Shizu_Integer32 v,
    Shizu_Boolean* sign,
    uint8_t** p,
    uint32_t* n
  );

#endif // SHIZU_OBJECTS_BIGINTEGER_FROMINTEGER32_H_INCLUDED
