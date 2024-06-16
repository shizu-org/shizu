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
Compiler_Parser_visit
  (
    Shizu_State* state,
    Compiler_Parser* self
  )
{ }

static Shizu_TypeDescriptor const Compiler_Parser_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*)NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*)NULL,
  .visitType = NULL,
  .size = sizeof(Compiler_Parser),
  .visit = (Shizu_OnVisitCallback*)&Compiler_Parser_visit,
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .dispatchSize = sizeof(Compiler_Parser_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Compiler_Parser, Compiler_Object);

void
Compiler_Parser_construct
  (
    Shizu_State* state,
    Compiler_Parser* self
  )
{
  Shizu_Type* TYPE = Compiler_Parser_getType(state);
  Compiler_Object_construct(state, (Compiler_Object*)self);
  ((Shizu_Object*)self)->type = TYPE;
}

Compiler_Parser*
Compiler_Parser_create
  (
    Shizu_State* state
  )
{
  Shizu_Type* TYPE = Compiler_Parser_getType(state);
  Compiler_Parser* self = (Compiler_Parser*)Shizu_Gc_allocateObject(state, sizeof(Compiler_Parser));
  Compiler_Parser_construct(state, self);
  return self;
}
