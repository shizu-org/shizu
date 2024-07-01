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
#include "Shizu/Runtime/Module.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/String.h"

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

// fprintf, stdio
#include <stdio.h>

static void
Shizu_Module_finalize
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  );

static void
Shizu_Module_visit
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  );

static Shizu_TypeDescriptor const Shizu_Module_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Module),
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_Module_finalize,
  .visit = (Shizu_OnVisitCallback*)&Shizu_Module_visit,
  .dispatchSize = sizeof(Shizu_Module_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_Module, Shizu_Object);

static void
Shizu_Module_finalize
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{ 
  if (self->dl) {
    Shizu_State1_unrefDl(Shizu_State2_getState1(state),self->dl);
    self->dl = NULL;
  }
}

static void
Shizu_Module_visit
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{
  if (self->path) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->path);
  }
}

static void
Shizu_Module_ensureLibraryLoaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{
  if (!self->dl) {
    Shizu_String* zeroTerminator = Shizu_String_create(state, "", sizeof(char));
    Shizu_String* zeroTerminatedPath = Shizu_String_concatenate(state, self->path, zeroTerminator);
    self->dl = Shizu_State1_getOrLoadDl(Shizu_State2_getState1(state), Shizu_String_getBytes(state, zeroTerminatedPath), true);
    if (!self->dl) {
      fprintf(stderr, "unable to link `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, self->path), Shizu_String_getBytes(state, self->path));
      Shizu_State2_jump(state);
    }
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_CxxFunction* f = (Shizu_CxxFunction*)Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), self->dl, "Shizu_ModuleLibrary_load");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_load", (int)Shizu_String_getNumberOfBytes(state, self->path), Shizu_String_getBytes(state, self->path));
      Shizu_State2_jump(state);
    }
    Shizu_Value returnValue;
    Shizu_Value_setVoid(&returnValue, Shizu_Void_Void);
    Shizu_Value argumentValues[1];
    (*f)(state, &returnValue, 0, &argumentValues[0]);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State2_getState1(state), self->dl);
    self->dl = NULL;
    Shizu_State2_jump(state);
  }
}

Shizu_Module*
Shizu_Module_create
  (
    Shizu_State2* state,
    Shizu_String* path
  )
{
  Shizu_Module* self = (Shizu_Module*)Shizu_Gc_allocateObject(state, sizeof(Shizu_Module));
  self->path = path;
  self->dl = NULL;
  ((Shizu_Object*)self)->type = Shizu_Module_getType(state);
  return self;
}

Shizu_String*
Shizu_Module_getName
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  )
{
  Shizu_Module_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    char const* (*getName)(Shizu_State1*) = (char const * (*)(Shizu_State1*))Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), rendition->dl, "Shizu_ModuleLibrary_getName");
    if (!getName) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_getName", (int)Shizu_String_getNumberOfBytes(state, rendition->path), Shizu_String_getBytes(state, rendition->path));
      Shizu_State2_jump(state);
    }
    char const* p = getName(Shizu_State2_getState1(state));
    Shizu_String* name = Shizu_String_create(state, p, strlen(p));
    Shizu_State2_popJumpTarget(state);
    return name;
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }
}

Shizu_CxxFunction*
Shizu_Module_getUpdate
  (
    Shizu_State2* state,
    Shizu_Module* rendition
  )
{
  Shizu_Module_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_CxxFunction* f = (Shizu_CxxFunction*)Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), rendition->dl, "Shizu_ModuleLibrary_update");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_update", (int)Shizu_String_getNumberOfBytes(state, rendition->path), Shizu_String_getBytes(state, rendition->path));
      Shizu_State2_jump(state);
    }
    Shizu_State2_popJumpTarget(state);
    return f;
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_State2_jump(state);
  }
}

void
Shizu_Module_ensureLoaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{
  if (!self->dl) {
    Shizu_String* zeroTerminator = Shizu_String_create(state, "", sizeof(char));
    Shizu_String* zeroTerminatedPath = Shizu_String_concatenate(state, self->path, zeroTerminator);
    self->dl = Shizu_State1_getOrLoadDl(Shizu_State2_getState1(state), Shizu_String_getBytes(state, zeroTerminatedPath), true);
    if (!self->dl) {
      fprintf(stderr, "unable to link `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, self->path), Shizu_String_getBytes(state, self->path));
      Shizu_State2_jump(state);
    }
    //
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_CxxFunction* f = (Shizu_CxxFunction*)Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), self->dl, "Shizu_ModuleLibrary_load");
      if (!f) {
        fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_load", (int)Shizu_String_getNumberOfBytes(state, self->path), Shizu_String_getBytes(state, self->path));
        Shizu_State2_jump(state);
      }
      Shizu_Value returnValue;
      Shizu_Value_setVoid(&returnValue, Shizu_Void_Void);
      Shizu_Value argumentValues[1];
      (*f)(state, &returnValue, 0, &argumentValues[0]);
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), self->dl);
      self->dl = NULL;
      Shizu_State2_jump(state);
    }
  }
}

void
Shizu_Module_ensureUnloaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{
  if (self->dl) {
    //
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_CxxFunction* f = (Shizu_CxxFunction*)Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), self->dl, "Shizu_ModuleLibrary_unload");
      if (!f) {
        fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_unload", (int)Shizu_String_getNumberOfBytes(state, self->path), Shizu_String_getBytes(state, self->path));
        Shizu_State2_jump(state);
      }
      Shizu_Value returnValue;
      Shizu_Value_setVoid(&returnValue, Shizu_Void_Void);
      Shizu_Value argumentValues[1];
      (*f)(state, &returnValue, 0, &argumentValues[0]);
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), self->dl);
      self->dl = NULL;
      Shizu_State2_jump(state);
    }
    //
    Shizu_State1_unrefDl(Shizu_State2_getState1(state), self->dl);
    self->dl = NULL;
  }
}

Shizu_Boolean
Shizu_Module_isLoaded
  (
    Shizu_State2* state,
    Shizu_Module* self
  )
{ return NULL != self->dl; }
