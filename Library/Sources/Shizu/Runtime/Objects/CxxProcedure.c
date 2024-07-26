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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/Objects/CxxProcedure.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Gc.h"

// memcmp, memcpy
#include <string.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static void
Shizu_CxxProcedure_finalize
  (
    Shizu_State2* state,
    Shizu_CxxProcedure* self
  );

static Shizu_ObjectTypeDescriptor const Shizu_CxxProcedure_Type = {
  .postCreateType = (Shizu_PostCreateTypeCallback*) NULL,
  .preDestroyType = (Shizu_PreDestroyTypeCallback*) NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_CxxProcedure),
  .construct = NULL,
  .visit = (Shizu_OnVisitCallback*) NULL,
  .finalize = (Shizu_OnFinalizeCallback*) & Shizu_CxxProcedure_finalize,
  .dispatchSize = sizeof(Shizu_CxxProcedure_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Shizu.CxxProcedure", Shizu_CxxProcedure, Shizu_Object);

static void
Shizu_CxxProcedure_finalize
  (
    Shizu_State2* state,
    Shizu_CxxProcedure* self
  )
{
  if (self->dl) {
    Shizu_State1_unrefDl(Shizu_State2_getState1(state), self->dl);
    self->dl = NULL;
  }
}

void
Shizu_CxxProcedure_construct
  (
    Shizu_State2* state,
    Shizu_CxxProcedure* self,
    Shizu_CxxFunction* f,
    Shizu_Dl* dl
  )
{
  Shizu_Type* TYPE = Shizu_CxxProcedure_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->f = f;
  self->dl = dl;
  if (self->dl) {
    Shizu_State1_refDl(Shizu_State2_getState1(state), self->dl);
  }
  ((Shizu_Object*)self)->type = TYPE;
}

Shizu_CxxProcedure*
Shizu_CxxProcedure_create
  (
    Shizu_State2* state,
    Shizu_CxxFunction* f,
    Shizu_Dl* dl
  )
{
  Shizu_Type* TYPE = Shizu_CxxProcedure_getType(state);
  Shizu_ObjectTypeDescriptor const* DESCRIPTOR = Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
  Shizu_CxxProcedure* SELF = (Shizu_CxxProcedure*)Shizu_Gc_allocateObject(state, DESCRIPTOR->size);
  Shizu_CxxProcedure_construct(state, SELF, f, dl);
  return SELF;
}
