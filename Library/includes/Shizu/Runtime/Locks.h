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

#if !defined(SHIZU_RUNTIME_LOCKS_H_INCLUDED)
#define SHIZU_RUNTIME_LOCKS_H_INCLUDED

#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Value.h"

void
Shizu_Object_lock
  (
    Shizu_State* state,
    Shizu_Object* self
  );

void
Shizu_Object_unlock
  (
    Shizu_State* state,
    Shizu_Object* self
  );

#endif // SHIZU_RUNTIME_LOCKS_H_INCLUDED
