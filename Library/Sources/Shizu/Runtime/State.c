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
#include "Shizu/Runtime/State.h"

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "Shizu/Runtime/DebugAssert.h"
#include "Shizu/Runtime/Module.h"
#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Objects/List.h"
#include "Shizu/Runtime/Objects/Environment.h"
#include "Shizu/Runtime/Objects/String.h"
#include "Shizu/Runtime/Stack.private.h"
#include "Shizu/Runtime/Type.private.h"
#include "Shizu/Runtime/getWorkingDirectory.h"
#include "idlib/file_system.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// strcmp
#include <string.h>



struct Shizu_State {
  int referenceCount;
  Shizu_State2* state2;
  Shizu_Stack* stack;
  idlib_process* process;
  /// The globals environment.
  Shizu_Environment* globals;
  /// List of modules.
  Shizu_List* modules;
};

static Shizu_State* g_singleton = NULL;

#define NAME "Shizu.State.Singleton"

int
Shizu_State_create
  (
    Shizu_State** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  if (!g_singleton) {
    Shizu_State* self = malloc(sizeof(Shizu_State));
    if (!self) {
      return 1;
    }
    if (idlib_process_acquire(&self->process)) {
      free(self);
      return 1;
    }
    if (Shizu_State2_acquire(&self->state2)) {
      idlib_process_relinquish(self->process);
      self->process = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    self->globals = NULL;
    self->stack = NULL;
    self->modules = NULL;

    Shizu_JumpTarget jumpTarget;
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->stack = Shizu_Stack_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_State2_relinquish(self->state2);
      self->state2 = NULL;
      idlib_process_relinquish(self->process);
      self->process = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Environment* globals = Shizu_Environment_create(self);
      Shizu_Object_lock(Shizu_State_getState1(self), Shizu_State_getLocks(self), (Shizu_Object*)globals);
      self->globals = globals;
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Stack_shutdown(self, self->stack);
      self->stack = NULL;
      Shizu_State2_relinquish(self->state2);
      self->state2 = NULL;
      idlib_process_relinquish(self->process);
      self->process = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_List* modules = Shizu_List_create(self);
      Shizu_Object_lock(Shizu_State_getState1(self), Shizu_State_getLocks(self), (Shizu_Object*)modules);
      self->modules = modules;
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Object_unlock(Shizu_State_getState1(self), Shizu_State_getLocks(self), (Shizu_Object*)self->globals);
      self->globals = NULL;
      Shizu_Stack_shutdown(self, self->stack);
      self->stack = NULL;
      Shizu_State2_relinquish(self->state2);
      self->state2 = NULL;
      idlib_process_relinquish(self->process);
      self->process = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    self->referenceCount = 0;
    g_singleton = self;
  }
  g_singleton->referenceCount++;
  *RETURN = g_singleton;
  return 0;
}

int
Shizu_State_destroy
  (
    Shizu_State* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    Shizu_Gc_run(self);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      size_t sizeValue = Shizu_List_getSize(self, self->modules);
      for (Shizu_Integer32 i = sizeValue; i > 0; --i) {
        Shizu_Value elementValue = Shizu_List_getValue(self, self->modules, i - 1);
        if (!Shizu_Value_isObject(&elementValue)) {
          // Developer error if this occurs.
          fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        Shizu_Object* elementObject = Shizu_Value_getObject(&elementValue);
        if (!Shizu_Types_isSubTypeOf(Shizu_State_getState1(self), Shizu_State_getTypes(self), Shizu_State_getObjectType(self, elementObject), Shizu_Module_getType(self))) {
          // Developer error if this occurs.
          fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        Shizu_Module* elementModule = (Shizu_Module*)elementObject;
        Shizu_Module_ensureUnloaded(self, elementModule);
      }
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      // Developer error if this occurs.
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }

    Shizu_Object_unlock(Shizu_State_getState1(self), Shizu_State_getLocks(self), (Shizu_Object*)self->modules);
    self->modules = NULL;
    Shizu_Object_unlock(Shizu_State_getState1(self), Shizu_State_getLocks(self), (Shizu_Object*)self->globals);
    self->globals = NULL;
    Shizu_Gc_run(self);
    

    Shizu_Stack_shutdown(self, self->stack);
    self->stack = NULL;
    Shizu_Gc_run(self);
    Shizu_State2_relinquish(self->state2);
    self->state2 = NULL;
    idlib_process_relinquish(self->process);
    self->process = NULL;
    g_singleton = NULL;
    free(self);
  }
  return 0;
}

void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_pushJumpTarget(Shizu_State2_getState1(self->state2), jumpTarget);
}

void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_popJumpTarget(Shizu_State2_getState1(self->state2));
}

Shizu_NoReturn() void
Shizu_State_jump
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_jump(Shizu_State2_getState1(self->state2));
}

void
Shizu_State_setStatus
  (
    Shizu_State* self,
    Shizu_Status error
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setStatus(Shizu_State2_getState1(self->state2), error);
}

Shizu_Status
Shizu_State_getStatus
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getStatus(Shizu_State2_getState1(self->state2));
}

void
Shizu_State_setProcessExitRequested
  (
    Shizu_State* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setProcessExitRequested(Shizu_State2_getState1(self->state2), processExitRequested);
}

bool
Shizu_State_getProcessExitRequested
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getProcessExitRequested(Shizu_State2_getState1(self->state2));
}

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* self,
    char const* nameBytes,
    size_t numberOfNameBytes
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != nameBytes);
  return Shizu_Types_getTypeByName(Shizu_State2_getState1(self->state2), Shizu_State_getTypes(self), nameBytes, numberOfNameBytes);
}

Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* bytes,
    size_t numberOfBytes,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* typeDescriptor
  )
{ return Shizu_Types_createType(Shizu_State2_getState1(self->state2), Shizu_State2_getTypes(self->state2), bytes, numberOfBytes, parentType, dl, typeDestroyed, typeDescriptor); }

Shizu_Gc*
Shizu_State_getGc
  (
    Shizu_State* self
  )
{ return Shizu_State2_getGc(self->state2); }

Shizu_Locks*
Shizu_State_getLocks
  (
    Shizu_State* self
  )
{ return Shizu_State2_getLocks(self->state2); }

Shizu_Stack*
Shizu_State_getStack
  (
    Shizu_State* self
  )
{ return self->stack; }

Shizu_Types*
Shizu_State_getTypes
  (
    Shizu_State* self
  )
{ return Shizu_State2_getTypes(self->state2); }

Shizu_Dl*
Shizu_State_getOrLoadDl
  (
    Shizu_State* self,
    char const* path,
    bool load
  )
{ return Shizu_State1_getOrLoadDl(Shizu_State2_getState1(self->state2), path, load); }

Shizu_Dl*
Shizu_State_getDlByName
  (
    Shizu_State* self,
    char const* name
  )
{ return Shizu_State1_getDlByName(Shizu_State2_getState1(self->state2), name); }

Shizu_Dl*
Shizu_State_getDlByAdr
  (
    Shizu_State* self,
    void *p
  )
{ return Shizu_State1_getDlByAdr(Shizu_State2_getState1(self->state2), p); }

void
Shizu_Dl_ref
  (
    Shizu_State* self,
    Shizu_Dl* dl
  )
{ Shizu_State1_refDl(Shizu_State2_getState1(self->state2), dl); }

void
Shizu_Dl_unref
  (
    Shizu_State* self,
    Shizu_Dl* dl
  )
{ Shizu_State1_unrefDl(Shizu_State2_getState1(self->state2), dl); }

void*
Shizu_Dl_getSymbol
  (
    Shizu_State* self,
    Shizu_Dl* dl,
    char const* name
  )
{ return Shizu_State1_getDlSymbol(Shizu_State2_getState1(self->state2), dl, name); }

Shizu_State1*
Shizu_State_getState1
  (
    Shizu_State* self
  )
{
  return Shizu_State2_getState1(self->state2);
}

Shizu_State2*
Shizu_State_getState2
  (
    Shizu_State* self
  )
{
  return self->state2;
}

typedef struct LoadModulesContext {
  Shizu_State* state;
  Shizu_List* list;
  Shizu_String* prefix;
} LoadModulesContext;

static bool
loadModulesCallback
  (
    LoadModulesContext* context,
    char const* bytes,
    size_t numberOfBytes
  )
{
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(context->state, &jumpTarget);
  bool result = true;
  if (!setjmp(jumpTarget.environment)) {
    Shizu_String* string = Shizu_String_create(context->state, bytes, numberOfBytes);
    string = Shizu_String_concatenate(context->state, context->prefix, string);
    Shizu_List_appendObject(context->state, context->list, (Shizu_Object*)string);
    Shizu_State_popJumpTarget(context->state);
  } else {
    Shizu_State_popJumpTarget(context->state);
    return false;
  }
  return true;
}

static bool
isModule
  (
    Shizu_State* state,
    Shizu_String* path
  )
{
  Shizu_String* extension = Shizu_String_create(state, Shizu_OperatingSystem_DlExtension, strlen(Shizu_OperatingSystem_DlExtension));
  // First level condition: Must end with the operating systems dynamic library extension (".dll" in Windows, ".so" under Linux, etc.).
  if (!Shizu_String_endsWith(state, path, extension)) {
    return false;
  }
  // Second level condition: Load the dll.
  Shizu_Dl* dl = Shizu_State_getOrLoadDl(state, Shizu_String_getBytes(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "", 1))), true);
  if (!dl) {
    return false;
  }
  // Third level condition: Must provide `Shizu_ModuleLibrary_getName`, `Shizu_ModuleLibrary_load`, `Shizu_ModuleLibrary_unload`.
  {
    char const *(*f)(Shizu_State1*) = NULL;
    f = (char const* (*)(Shizu_State1*))Shizu_Dl_getSymbol(state, dl, "Shizu_ModuleLibrary_getName");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_getName",
                      (int)Shizu_String_getNumberOfBytes(state, path),
                      Shizu_String_getBytes(state, path));
      Shizu_Dl_unref(state, dl);
      dl = NULL;
      Shizu_State_jump(state);
    }
  }
  {
    void (*f)(Shizu_State*) = (void (*)(Shizu_State*))Shizu_Dl_getSymbol(state, dl, "Shizu_ModuleLibrary_load");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_load",
                      (int)Shizu_String_getNumberOfBytes(state, path),
                      Shizu_String_getBytes(state, path));
      Shizu_Dl_unref(state, dl);
      dl = NULL;
      Shizu_State_jump(state);
    }
  }
  {
    void (*f)(Shizu_State*) = (void (*)(Shizu_State*))Shizu_Dl_getSymbol(state, dl, "Shizu_ModuleLibrary_unload");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_unload",
                      (int)Shizu_String_getNumberOfBytes(state, path),
                      Shizu_String_getBytes(state, path));
      Shizu_Dl_unref(state, dl);
      dl = NULL;
      Shizu_State_jump(state);
    }
  }
  Shizu_Dl_unref(state, dl);
  dl = NULL;
  return true;
}

void
Shizu_State_ensureModulesLoaded
  (
    Shizu_State* state
  )
{
  Shizu_String* workingDirectory = Shizu_getWorkingDirectory(state);
  Shizu_List* modules = Shizu_List_create(state);
  LoadModulesContext context;
  context.state = state;
  context.list = modules;
  context.prefix = Shizu_String_concatenate(state, workingDirectory, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator, strlen(Shizu_OperatingSystem_DirectorySeparator)));
  if (idlib_enumerate_files(Shizu_String_getBytes(state, Shizu_String_concatenate(state, Shizu_getWorkingDirectory(state), Shizu_String_create(state, "", 1))),
                            &context, (idlib_enumerate_files_callback_function*)&loadModulesCallback, true, true)) {
    Shizu_State_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State_jump(state);
  }
  size_t size = Shizu_List_getSize(state, modules);
  for (Shizu_Integer32 i = 0, n = (Shizu_Integer32)size; i < n; ++i) {
    Shizu_Value element = Shizu_List_getValue(state, modules, i);
    if (isModule(state, (Shizu_String*)Shizu_Value_getObject(&element))) {
      Shizu_Module* module = Shizu_Module_create(state, (Shizu_String*)Shizu_Value_getObject(&element));
      Shizu_List_appendObject(state, state->modules, (Shizu_Object*)module);
      Shizu_Module_ensureLoaded(state, module);
    }
  }
}

Shizu_Environment*
Shizu_State_getGlobals
  (
    Shizu_State* state
  )
{ return state->globals; }
