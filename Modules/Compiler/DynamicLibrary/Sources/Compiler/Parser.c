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
#include "Compiler/Parser.h"

#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"

static void
Shizu_Runtime_Compiler_Parser_visit
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Parser* self
  )
{ }

static Shizu_TypeDescriptor const Shizu_Runtime_Compiler_Parser_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Runtime_Compiler_Parser),
  .visit = (Shizu_OnVisitCallback*)&Shizu_Runtime_Compiler_Parser_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Shizu_Runtime_Compiler_Parser_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_Runtime_Compiler_Parser, Shizu_Runtime_Compiler_Object);

void
Shizu_Runtime_Compiler_Parser_construct
  (
    Shizu_State* state,
    Shizu_Runtime_Compiler_Parser* self
  )
{
  Shizu_Type* TYPE = Shizu_Runtime_Compiler_Parser_getType(state);
  Shizu_Runtime_Compiler_Object_construct(state, (Shizu_Runtime_Compiler_Object*)self);
  ((Shizu_Object*)self)->type = TYPE;
}

Shizu_Runtime_Compiler_Parser*
Shizu_Runtime_Compiler_Parser_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Shizu_Runtime_Compiler_Parser_getType(state);
  Shizu_Runtime_Compiler_Parser* self = (Shizu_Runtime_Compiler_Parser*)Shizu_Gc_allocateObject(state, sizeof(Shizu_Runtime_Compiler_Parser));
  Shizu_Runtime_Compiler_Parser_construct(state, self);
  return self;
}
