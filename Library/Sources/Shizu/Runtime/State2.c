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
#include "Shizu/Runtime/State2.h"


// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// strlen
#include <string.h>

#include "idlib/process.h"
#include "idlib/file_system.h"

#include "Shizu/Runtime/CxxUtilities.h"
#include "Shizu/Runtime/State1.h"

#include "Shizu/Runtime/getWorkingDirectory.h"
#include "Shizu/Runtime/Gc.private.h"
#include "Shizu/Runtime/Locks.private.h"
#include "Shizu/Runtime/Type.private.h"
#include "Shizu/Runtime/Stack.private.h"
#include "Shizu/Runtime/Module.h"
#include "Shizu/Runtime/Objects/List.private.h"
#include "Shizu/Runtime/Objects/Environment.private.h"
#include "Shizu/Runtime/Objects/WeakReference.private.h"
#include "Shizu/Runtime/Objects/String.h"

struct Shizu_State2 {
  int referenceCount;
  /// The number of components that are already initialized.
  size_t numberOfInitializedComponents;

  Shizu_State1* state1;
  idlib_process* process;

  /// The "types" state.
  Shizu_Types* types;
  /// The "gc" state.
  Shizu_Gc* gc;
  /// The "locks" state.
  Shizu_Locks* locks;
  /// The "stack" state.
  Shizu_Stack* stack;
  /// The "weak references" state.
  Shizu_WeakReferences* weakReferences;
  /// The global environment.
  Shizu_Environment* globalEnvironment;

  /// List of modules.
  Shizu_List* modules;
};

static int
Shizu_State2_create
  (
    Shizu_State2** RETURN
  );

static int
Shizu_State2_destroy
  (
    Shizu_State2* self
  );


static void startup1(Shizu_State2* state) {
  if (idlib_process_acquire(&state->process)) {
    Shizu_State1_setStatus(state->state1, Shizu_Status_EnvironmentFailed);
    Shizu_State1_jump(state->state1);
  }
}

static void shutdown1(Shizu_State2* state) {
  idlib_process_relinquish(state->process);
  state->process = NULL;
}

static void startup2(Shizu_State2* state) {
  state->types = Shizu_Types_startup(state->state1);
}

static void shutdown2(Shizu_State2* state) {
  Shizu_Types_shutdown(state->state1, state->types);
  state->types = NULL;
}

static void startup3(Shizu_State2* state) {
  state->gc = Shizu_Gc_create(state->state1);
}

static void shutdown3(Shizu_State2* state) {
  Shizu_debugAssert(NULL == state->stack);
  Shizu_debugAssert(NULL == state->locks);
  Shizu_Gc_destroy(state->state1, state->gc);
  state->gc = NULL;
}

static void startup4(Shizu_State2* state) {
  state->locks = Shizu_Locks_create(state->state1);
  Shizu_JumpTarget jumpTarget;
  //
  Shizu_State1_pushJumpTarget(state->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Gc_addPreMarkHook(state->state1, state->gc, state->locks, &Shizu_Locks_notifyPreMark);
    Shizu_State1_popJumpTarget(state->state1);
  } else {
    Shizu_State1_popJumpTarget(state->state1);
    Shizu_Locks_destroy(state->state1, state->locks);
    state->locks = NULL;
    Shizu_State1_jump(state->state1);
  }
  //
  Shizu_State1_pushJumpTarget(state->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Gc_addObjectFinalizeHook(state->state1, state->gc, state->locks, &Shizu_Locks_notifyObjectFinalize);
    Shizu_State1_popJumpTarget(state->state1);
  } else {
    Shizu_State1_popJumpTarget(state->state1);
    Shizu_Gc_removePreMarkHook(state->state1, state->gc, state->locks, &Shizu_Locks_notifyPreMark);
    Shizu_Locks_destroy(state->state1, state->locks);
    state->locks = NULL;
    Shizu_State1_jump(state->state1);
  }
}

static void shutdown4(Shizu_State2* state) {
  Shizu_debugAssert(NULL == state->stack);
  size_t size;
  size = Shizu_Locks_getSize(state->state1, state->locks);
  if (size > 0) {
    fprintf(stderr, "%s: %d: warning: expected number of locks is %zu. received number of locks is %zu\n", __FILE__, __LINE__, (size_t)0, size);
  }
  Shizu_Gc_removeObjectFinalizeHook(state->state1, state->gc, state->locks, &Shizu_Locks_notifyObjectFinalize);
  Shizu_Gc_removePreMarkHook(state->state1, state->gc, state->locks, &Shizu_Locks_notifyPreMark);
  Shizu_Locks_destroy(state->state1, state->locks);
  state->locks = NULL;
}

static void startup5(Shizu_State2* state) {
  state->weakReferences = Shizu_WeakReferences_create(state->state1);
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Gc_addObjectFinalizeHook(state->state1, state->gc, state->weakReferences, &Shizu_WeakReferenceState_notifyObjectFinalize);
    Shizu_State1_popJumpTarget(state->state1);
  } else {
    Shizu_State1_popJumpTarget(state->state1);
    Shizu_WeakReferences_destroy(state->state1, state->weakReferences);
    state->weakReferences = NULL;
    Shizu_State1_jump(state->state1);
  }
}

static void shutdown5(Shizu_State2* state) {
  Shizu_Gc_removeObjectFinalizeHook(state->state1, state->gc, state->weakReferences, &Shizu_WeakReferenceState_notifyObjectFinalize);
  Shizu_WeakReferences_destroy(state->state1, state->weakReferences);
  state->weakReferences = NULL;
}

static void startup6(Shizu_State2* state) {
  state->stack = Shizu_Stack_create(state->state1);
  Shizu_JumpTarget jumpTarget;
  Shizu_State1_pushJumpTarget(state->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Gc_addPreMarkHook(state->state1, state->gc, state->stack, &Shizu_Stack_notifyPreMark);
    Shizu_State1_popJumpTarget(state->state1);
  } else {
    Shizu_State1_popJumpTarget(state->state1);
    Shizu_Stack_destroy(state->state1, state->stack);
    state->stack = NULL;
    Shizu_State1_jump(state->state1);
  }
}

static void shutdown6(Shizu_State2* state) {
  size_t size;
  size = Shizu_Stack_getSize(state->state1, state->stack);
  if (size > 0) {
    fprintf(stderr, "%s: %d: warning: expected number of stack elements is %zu. received number of stack elements is %zu. Clearing stack.\n", __FILE__, __LINE__, (size_t)0, size);
    Shizu_Stack_clear(state->state1, state->stack);
  }
  Shizu_Gc_removePreMarkHook(state->state1, state->gc, state->stack, &Shizu_Stack_notifyPreMark);
  Shizu_Stack_destroy(state->state1, state->stack);
  state->stack = NULL;
}

static void startup7(Shizu_State2* state) {
  Shizu_Environment* globalEnvironment = Shizu_Environment_create(state);
  Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)globalEnvironment);
  state->globalEnvironment = globalEnvironment;
}

static void shutdown7(Shizu_State2* state) {
  Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)state->globalEnvironment);
  state->globalEnvironment = NULL;
  Shizu_Gc_SweepInfo sweepInfo = { .dead = 0, .live = 0 };
  do {
    sweepInfo.dead = 0;
    sweepInfo.live = 0;
    Shizu_Gc_run(state, Shizu_State2_getGc(state), &sweepInfo);
  } while (sweepInfo.dead);
}

static void startup8(Shizu_State2* state) {
  Shizu_List* modules = Shizu_List_create(state);
  Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)modules);
  state->modules = modules;
}

static void shutdown8(Shizu_State2* state) {
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    for (Shizu_Integer32 i = Shizu_List_getSize(state, state->modules); i > 0; --i) {
      Shizu_Value elementValue = Shizu_List_getValue(state, state->modules, i - 1);
      if (!Shizu_Value_isObject(&elementValue)) {
        // Developer error if this occurs.
        Shizu_unreachableCodeReached(__FILE__, __LINE__);
      }
      Shizu_Object* elementObject = Shizu_Value_getObject(&elementValue);
      if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_State2_getObjectType(state, elementObject), Shizu_Module_getType(state))) {
        // Developer error if this occurs.
        Shizu_unreachableCodeReached(__FILE__, __LINE__);
      }
      Shizu_Module* elementModule = (Shizu_Module*)elementObject;
      Shizu_Module_ensureUnloaded(state, elementModule);
    }
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    // Developer error if this occurs.
    Shizu_unreachableCodeReached(__FILE__, __LINE__);
  }
  Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)state->modules);
  state->modules = NULL;
}

typedef struct ComponentDescriptor {
  void (*startup)(Shizu_State2*);
  void (*shutdown)(Shizu_State2*);
} ComponentDescriptor;

static const ComponentDescriptor g_components[] =
{
  {
    .startup = &startup1,
    .shutdown = &shutdown1,
  },
  {
     .startup = &startup2,
    .shutdown = &shutdown2,
  },
  {
    .startup = &startup3,
    .shutdown = &shutdown3,
  },
  {
    .startup = &startup4,
    .shutdown = &shutdown4,
  },
  {
    .startup = &startup5,
    .shutdown = &shutdown5,
  },
  {
    .startup = &startup6,
    .shutdown = &shutdown6,
  },
  {
    .startup = &startup7,
    .shutdown = &shutdown7,
  },
  {
    .startup = &startup8,
    .shutdown = &shutdown8,
  },
};

static const size_t g_numberOfComponents = sizeof(g_components) / sizeof(ComponentDescriptor);

static int
Shizu_State2_create
  (
    Shizu_State2** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  Shizu_State1* state1 = NULL;
  if (Shizu_State1_acquire(&state1)) {
    return 1;
  }
  Shizu_State2* self = Shizu_State1_allocate(state1, sizeof(Shizu_State2));
  if (!self) {
    Shizu_State1_relinquish(state1);
    state1 = NULL;
    return 1;
  }
  self->process = NULL;
  self->types = NULL;
  self->gc = NULL;
  self->locks = NULL;
  self->stack = NULL;
  self->globalEnvironment = NULL;
  self->modules = NULL;


  self->state1 = state1;
  self->numberOfInitializedComponents = 0;
  if (idlib_process_acquire(&self->process)) {
    Shizu_State1* state1 = self->state1;
    Shizu_State1_deallocate(state1, self);
    self = NULL;
    Shizu_State1_relinquish(state1);
    state1 = NULL;
    return 1;
  }
  Shizu_JumpTarget jumpTarget;
  //
  Shizu_State1_pushJumpTarget(self->state1, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    for (size_t i = 0, n = g_numberOfComponents; i < n; ++i) {
      g_components[i].startup(self);
      self->numberOfInitializedComponents++;
    }
    Shizu_State1_popJumpTarget(self->state1);
  } else {
    Shizu_State1_popJumpTarget(self->state1);
    while (self->numberOfInitializedComponents) {
      g_components[--self->numberOfInitializedComponents].shutdown(self);
    }
    Shizu_State1* state1 = self->state1;
    Shizu_State1_deallocate(state1, self);
    self = NULL;
    Shizu_State1_relinquish(state1);
    state1 = NULL;
    return 1;
  }
   *RETURN = self;
  return 0;
}

static int
Shizu_State2_destroy
  (
    Shizu_State2* self
  )
{
  while (self->numberOfInitializedComponents) {
    g_components[--self->numberOfInitializedComponents].shutdown(self);
  }
  Shizu_State1* state1 = self->state1;
  Shizu_State1_deallocate(state1, self);
  self = NULL;
  Shizu_State1_relinquish(state1);
  state1 = NULL;
  return 0;
}

#define NAME "<Shizu.State2>"

int
Shizu_State2_acquire
  (
    Shizu_State2** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  idlib_process* process = NULL;
  if (idlib_process_acquire(&process)) {
    return 1;
  }
  Shizu_State2* self = NULL;
  int result = idlib_get_global(process, NAME, strlen(NAME), (void**)&self);
  if (result != IDLIB_SUCCESS && result != IDLIB_NOT_EXISTS) {
    idlib_process_relinquish(process);
    process = NULL;
    return 1;
  }
  if (result == IDLIB_NOT_EXISTS) {
    if (Shizu_State2_create(&self)) {
      idlib_process_relinquish(process);
      process = NULL;
      return 1;
    }
    self->referenceCount = 0;
    if (idlib_add_global(self->process, NAME, strlen(NAME), self)) {
      free(self);
      self = NULL;
      idlib_process_relinquish(process);
      process = NULL;
      return 1;
    }
  } else {
    idlib_process_relinquish(process);
    process = NULL;
  }
  self->referenceCount++;
  *RETURN = self;
  return 0;
}

int
Shizu_State2_relinquish
  (
    Shizu_State2* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    idlib_process* process = NULL;
    idlib_process_acquire(&process);
    Shizu_State2_destroy(self);
    self = NULL;
    idlib_remove_global(process, NAME, strlen(NAME));
    idlib_process_relinquish(process);
    process = NULL;
  }
  return 0;
}

void
Shizu_State2_pushJumpTarget
  (
    Shizu_State2* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != self->state1);
  Shizu_debugAssert(NULL != jumpTarget);
  Shizu_State1_pushJumpTarget(self->state1, jumpTarget);
}

void
Shizu_State2_popJumpTarget
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_popJumpTarget(self->state1);
}

Shizu_NoReturn() void
Shizu_State2_jump
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_jump(self->state1);
}

void
Shizu_State2_setStatus
  (
    Shizu_State2* self,
    Shizu_Status status
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setStatus(self->state1, status);
}

Shizu_Status
Shizu_State2_getStatus
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getStatus(self->state1);
}

void
Shizu_State2_setProcessExitRequested
  (
    Shizu_State2* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setProcessExitRequested(self->state1, processExitRequested);
}

bool
Shizu_State2_getProcessExitRequested
  (
    Shizu_State2* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getProcessExitRequested(self->state1);
}

Shizu_State1*
Shizu_State2_getState1
  (
    Shizu_State2* self
  )
{ return self->state1; }

Shizu_Types*
Shizu_State2_getTypes
  (
    Shizu_State2* self
  )
{ return self->types; }

Shizu_Gc*
Shizu_State2_getGc
  (
    Shizu_State2* self
  )
{ return self->gc; }

Shizu_Locks*
Shizu_State2_getLocks
  (
    Shizu_State2* self
  )
{ return self->locks;}

Shizu_Stack*
Shizu_State2_getStack
  (
    Shizu_State2* self
  )
{ return self->stack;}

Shizu_WeakReferences*
Shizu_State2_getWeakReferences
  (
    Shizu_State2* self
  )
{ return self->weakReferences; }

Shizu_Environment*
Shizu_State2_getGlobalEnvironment
  (
    Shizu_State2* self
  )
{ return self->globalEnvironment; }

typedef struct LoadModulesContext {
  Shizu_State2* state;
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
  Shizu_State2_pushJumpTarget(context->state, &jumpTarget);
  bool result = true;
  if (!setjmp(jumpTarget.environment)) {
    Shizu_String* string = Shizu_String_create(context->state, bytes, numberOfBytes);
    string = Shizu_String_concatenate(context->state, context->prefix, string);
    Shizu_List_appendObject(context->state, context->list, (Shizu_Object*)string);
    Shizu_State2_popJumpTarget(context->state);
  } else {
    Shizu_State2_popJumpTarget(context->state);
    return false;
  }
  return true;
}

static bool
isModule
  (
    Shizu_State2* state,
    Shizu_String* path
  )
{
  Shizu_String* extension = Shizu_String_create(state, Shizu_OperatingSystem_DlExtension, strlen(Shizu_OperatingSystem_DlExtension));
  // First level condition: Must end with the operating systems dynamic library extension (".dll" in Windows, ".so" under Linux, etc.).
  if (!Shizu_String_endsWith(state, path, extension)) {
    return false;
  }
  // Second level condition: Load the dll.
  Shizu_Dl* dl = Shizu_State1_getOrLoadDl(Shizu_State2_getState1(state), Shizu_String_getBytes(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "", 1))), true);
  if (!dl) {
    return false;
  }
  // Third level condition: Must provide `Shizu_ModuleLibrary_getName`, `Shizu_ModuleLibrary_load`, `Shizu_ModuleLibrary_unload`.
  {
    char const* (*f)(Shizu_State1*) = NULL;
    f = (char const* (*)(Shizu_State1*))Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), dl, "Shizu_ModuleLibrary_getName");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_getName",
        (int)Shizu_String_getNumberOfBytes(state, path),
        Shizu_String_getBytes(state, path));
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
      dl = NULL;
      Shizu_State2_jump(state);
    }
  }
  {
    void (*f)(Shizu_State2*) = (void (*)(Shizu_State2*))Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), dl, "Shizu_ModuleLibrary_load");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_load",
        (int)Shizu_String_getNumberOfBytes(state, path),
        Shizu_String_getBytes(state, path));
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
      dl = NULL;
      Shizu_State2_jump(state);
    }
  }
  {
    void (*f)(Shizu_State2*) = (void (*)(Shizu_State2*))Shizu_State1_getDlSymbol(Shizu_State2_getState1(state), dl, "Shizu_ModuleLibrary_unload");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Shizu_ModuleLibrary_unload",
        (int)Shizu_String_getNumberOfBytes(state, path),
        Shizu_String_getBytes(state, path));
      Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
      dl = NULL;
      Shizu_State2_jump(state);
    }
  }
  Shizu_State1_unrefDl(Shizu_State2_getState1(state), dl);
  dl = NULL;
  return true;
}

void
Shizu_State2_ensureModulesLoaded
  (
    Shizu_State2* state
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
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
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

Shizu_Type*
Shizu_State2_getObjectType
  (
    Shizu_State2* self,
    Shizu_Object* object
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != object);
  Shizu_debugAssert(NULL != object->type);
  return object->type;
}

Shizu_Object_Dispatch*
Shizu_State2_getObjectDispatch
  (
    Shizu_State2* state,
    Shizu_Object* object
  )
{
  Shizu_Type* type = Shizu_State2_getObjectType(state, object);
  return type->objectType.dispatch;
}
