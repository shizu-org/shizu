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

#include "Compiler/Object.h"

static Shizu_ObjectTypeDescriptor const Compiler_Object_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Object),
  .visit = (Shizu_OnVisitCallback*)NULL,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Object_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Compiler_Object, Shizu_Object);

void
Compiler_Object_construct
  (
    Shizu_State2* state,
    Compiler_Object* self
  )
{
  Shizu_Type* TYPE = Compiler_Object_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  ((Shizu_Object*)self)->type = TYPE;
}
