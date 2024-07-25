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
#include "Shizu/Runtime/Gc.private.h"

#include "Shizu/Runtime/State2.h"
#include "Shizu/Runtime/Object.h"
#include "Shizu/Runtime/Type.private.h"

// stderr, fprintf
#include <stdio.h>

static void
Shizu_GcxInterface_finalizeCallback
  (
    void* finalizeContext,
    void* object
  );

static void
Shizu_GcxInterface_visitCallback
  (
    void* visitContext,
    void* object
  );

#define Shizu_Object_Flags_White (1)

#define Shizu_Object_Flags_Black (2)

#define Shizu_Object_Flags_Gray (Shizu_Object_Flags_White|Shizu_Object_Flags_Black)

Shizu_Gc*
Shizu_Gc_create
  (
    Shizu_State2* state
  )
{
  if (Shizu_Gcx_startup()) {
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  Shizu_Gc* self = Shizu_State1_allocate(Shizu_State2_getState1(state), sizeof(Shizu_Gc));
  if (!self) {
    Shizu_Gcx_shutdown();
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  if (Shizu_Gcx_registerType("Shizu.GcxInterface.Object", strlen("Shizu.GcxInterface.Object"), state, &Shizu_GcxInterface_visitCallback, state, &Shizu_GcxInterface_finalizeCallback)) {
    Shizu_Gcx_shutdown();
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self);
    self = NULL;
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }
  if (Shizu_Gcx_acquireType("Shizu.GcxInterface.Object", strlen("Shizu.GcxInterface.Object"), &self->type)) {
    Shizu_Gcx_unregisterType("Shizu.GcxInterface.Object", strlen("Shizu.GcxInterface.Object"));
    Shizu_Gcx_shutdown();
    Shizu_State1_deallocate(Shizu_State2_getState1(state), self);
    self = NULL;
    Shizu_State2_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State2_jump(state);
  }

  self->all = NULL;
  self->gray = NULL;
  self->preMarkHooks.nodes = NULL;
  self->preMarkHooks.running = false;
  self->objectFinalizeHooks.nodes = NULL;
  self->objectFinalizeHooks.running = false;

  return self;
}

void
Shizu_Gc_destroy
  (
    Shizu_State2* state,
    Shizu_Gc* self
  )
{
  if (self->preMarkHooks.nodes) {
    fprintf(stderr, "%s: %d: warning: premark hook list not empty\n", __FILE__, __LINE__);
  }
  if (self->gray) {
    fprintf(stderr, "%s: %d: warning: gray list not empty\n", __FILE__, __LINE__);
  }
  if (self->all) {
    fprintf(stderr, "%s: %d: warning: all list not empty\n", __FILE__, __LINE__);
  }
  if (self->preMarkHooks.nodes) {
    size_t live = 0;
    do {
      PreMarkHookNode* node = self->preMarkHooks.nodes;
      self->preMarkHooks.nodes = node->next;
      if (!node->dead) {
        live++;
      }
      Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
    } while (self->preMarkHooks.nodes);
    fprintf(stderr, "%s: %d: warning: pre mark hook node list not empty\n", __FILE__, __LINE__);
  }
  if (self->objectFinalizeHooks.nodes) {
    size_t live = 0;
    do {
      ObjectFinalizeHookNode* node = self->objectFinalizeHooks.nodes;
      self->objectFinalizeHooks.nodes = node->next;
      if (!node->dead) {
        live++;
      }
      Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
    } while (self->objectFinalizeHooks.nodes);
    if (live) {
      fprintf(stderr, "%s: %d: warning: object finalize hook node list not empty\n", __FILE__, __LINE__);
    }
  }
  Shizu_Gcx_relinquishType(self->type);
  self->type = NULL;
  Shizu_Gcx_unregisterType("Shizu.GcxInterface.Object", strlen("Shizu.GcxInterface.Object"));
  Shizu_State1_deallocate(Shizu_State2_getState1(state), self);
  self = NULL;
  Shizu_Gcx_shutdown();
}

Shizu_Object*
Shizu_Gc_allocateObject
  (
    Shizu_State2* state,
    size_t size
  )
{
#if Shizu_Configuration_InstructionSetArchitecture_X64 == Shizu_Configuration_InstructionSetArchitecture
  Shizu_Cxx_staticAssert(sizeof(Shizu_Object) % 8 == 0, "`Shizu_Object` must be a multiple of 8");
#elif Shizu_Configuration_InstructionSetArchitecture_X86 == Shizu_Configuration_InstructionSetArchitecture
  Shizu_Cxx_staticAssert(sizeof(Shizu_Object) % 4 == 0, "`Shizu_Object` must be a multiple of 4");
#else
  #error("instruction set architecture not (yet) supported")
#endif
  if (size < sizeof(Shizu_Object)) {
    fprintf(stderr, "%s:%d: size `%zu` is smaller than sizeof(Shizu_Object) = `%zu`\n", __FILE__, __LINE__, size, sizeof(Shizu_Object));
    Shizu_State1_setStatus(Shizu_State2_getState1(state), 1);
    Shizu_State1_jump(Shizu_State2_getState1(state));
  }
  Shizu_Object* self = NULL;
  if (Shizu_Gcx_allocate(size, Shizu_State2_getGc(state)->type, &self)) {
    fprintf(stderr, "%s:%d: unable to allocate `%zu` Bytes\n", __FILE__, __LINE__, size);
    Shizu_State1_setStatus(Shizu_State2_getState1(state), Shizu_Status_AllocationFailed);
    Shizu_State1_jump(Shizu_State2_getState1(state));
  }
  self->type = Shizu_Object_getType(state);
  return self;
}

static void
notifyObjectFinalizeHooks
  (
    Shizu_State2* state,
    Shizu_Gc* self,
    Shizu_Object* object
  )
{
  self->objectFinalizeHooks.running = true;
  size_t dead = 0;
  ObjectFinalizeHookNode* node = self->objectFinalizeHooks.nodes;
  while (node) {
    if (node->dead) {
      dead++;
      continue;
    }
    node->function(Shizu_State2_getState1(state), self, node->context, object);
    node = node->next;
  }
  self->objectFinalizeHooks.running = false;
  // This is very unlikely to happen.
  if (dead > 0) {
    ObjectFinalizeHookNode** previous = &self->objectFinalizeHooks.nodes;
    ObjectFinalizeHookNode* current = self->objectFinalizeHooks.nodes;
    while (current) {
      if (current->dead) {
        ObjectFinalizeHookNode* node = current;
        *previous = current->next;
        current = current->next;
        Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
      } else {
        previous = &current->next;
        current = current->next;
      }
      node->function(Shizu_State2_getState1(state), self, node->context, object);
    }
  }
}

static void
notifyPreMarkHooks
  (
    Shizu_State2* state,
    Shizu_Gc* self
  )
{
  self->preMarkHooks.running = true;
  size_t dead = 0;
  PreMarkHookNode* node = self->preMarkHooks.nodes;
  while (node) {
    if (node->dead) {
      dead++;
      continue;
    }
    node->function(Shizu_State2_getState1(state), self, node->context);
    node = node->next;
  }
  self->preMarkHooks.running = false;
  // This is very unlikely to happen.
  if (dead > 0) {
    PreMarkHookNode** previous = &self->preMarkHooks.nodes;
    PreMarkHookNode* current = self->preMarkHooks.nodes;
    while (current) {
      if (current->dead) {
        PreMarkHookNode* node = current;
        *previous = current->next;
        current = current->next;
        Shizu_State1_deallocate(Shizu_State2_getState1(state), node);
      } else {
        previous = &current->next;
        current = current->next;
      }
      node->function(Shizu_State2_getState1(state), self, node->context);
    }
  }
}

static void
Shizu_GcxInterface_finalizeCallback
  (
    void* finalizeContext,
    void* object
  )
{
  Shizu_State2* state1 = (Shizu_State2*)finalizeContext;
  Shizu_Object* object1 = (Shizu_Object*)object;

  // TODO: Shizu_Locks_notifyDestroy as well as Shizu_WeakReferences_notifyDestroy perform an object address hash lookup.
  //       Investigage if there is a relevant performance gain when only one hash table is used?
  notifyObjectFinalizeHooks(state1, Shizu_State2_getGc(state1), object);
  while (object1->type) {
    if (object1->type->objectType.descriptor->finalize) {
      object1->type->objectType.descriptor->finalize(state1, object);
    }
    object1->type = object1->type->objectType.parentType;
  }
}

static void
Shizu_GcxInterface_visitCallback
  (
    void* visitContext,
    void* object
  )
{
  Shizu_State2* state1 = (Shizu_State2*)visitContext;
  Shizu_Object* object1 = (Shizu_Object*)object;
  Shizu_Type* type = object1->type;
  while (type) {
    if (type->objectType.descriptor->visit) {
      type->objectType.descriptor->visit(state1, object1);
    }
    type = type->objectType.parentType;
  }
}

void
Shizu_Gc_run
  (
    Shizu_State2* state,
    Shizu_Gc* self,
    Shizu_Gc_SweepInfo* sweepInfo
  )
{
  Shizu_Cxx_Debug_assert(NULL == self->gray);
  notifyPreMarkHooks(state, self);
  size_t dead, live;
  Shizu_Gcx_run(&dead, &live);
  if (sweepInfo) {
    sweepInfo->dead = dead;
    sweepInfo->live = live;
  }
}

void
Shizu_Gc_visitObject
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Object* object
  )
{
  Shizu_Gcx_visit(object);
}

void
Shizu_Gc_visitValue
  (
    Shizu_State1* state1,
    Shizu_Gc* gc,
    Shizu_Value* value
  )
{
  switch (value->tag) {
    case Shizu_Value_Tag_Object: {
      Shizu_Gc_visitObject(state1, gc, (Shizu_Object*)Shizu_Value_getObject(value));
    } break;
    default: {
      /* Intentionally empty. */
    } break;
  }
}

void
Shizu_Gc_addPreMarkHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_PreMarkCallbackContext* context,
    Shizu_Gc_PreMarkCallbackFunction* function
  )
{
  PreMarkHookNode* node = Shizu_State1_allocate(state, sizeof(PreMarkHookNode));
  if (!node) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  node->next = gc->preMarkHooks.nodes;
  gc->preMarkHooks.nodes = node;
  node->context = context;
  node->function = function;
  node->dead = false;
}

void
Shizu_Gc_removePreMarkHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_PreMarkCallbackContext* context,
    Shizu_Gc_PreMarkCallbackFunction* function
  )
{
  if (gc->preMarkHooks.running) {
    PreMarkHookNode* node = gc->preMarkHooks.nodes;
    while (node) {
      if (node->context == context && node->function == function) {
        node->dead = true;
      }
      node = node->next;
    }
  } else {
    PreMarkHookNode** previous = &gc->preMarkHooks.nodes;
    PreMarkHookNode* current = gc->preMarkHooks.nodes;
    while (current) {
      if (current->dead || (current->context == context && current->function == function)) {
        *previous = current->next;
        PreMarkHookNode* node = current;
        current = current->next;
        Shizu_State1_deallocate(state, node);
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
  }
}

void
Shizu_Gc_addObjectFinalizeHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_ObjectFinalizeCallbackContext* context,
    Shizu_Gc_ObjectFinalizeCallbackFunction* function
  )
{
  ObjectFinalizeHookNode* node = Shizu_State1_allocate(state, sizeof(ObjectFinalizeHookNode));
  if (!node) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  node->next = gc->objectFinalizeHooks.nodes;
  gc->objectFinalizeHooks.nodes = node;
  node->context = context;
  node->function = function;
  node->dead = false;
}

void
Shizu_Gc_removeObjectFinalizeHook
  (
    Shizu_State1* state,
    Shizu_Gc* gc,
    Shizu_Gc_ObjectFinalizeCallbackContext* context,
    Shizu_Gc_ObjectFinalizeCallbackFunction* function
  )
{
  if (gc->objectFinalizeHooks.running) {
    ObjectFinalizeHookNode* node = gc->objectFinalizeHooks.nodes;
    while (node) {
      if (node->context == context && node->function == function) {
        node->dead = true;
      }
      node = node->next;
    }
  } else {
    ObjectFinalizeHookNode** previous = &gc->objectFinalizeHooks.nodes;
    ObjectFinalizeHookNode* current = gc->objectFinalizeHooks.nodes;
    while (current) {
      if (current->dead || (current->context == context && current->function == function)) {
        *previous = current->next;
        ObjectFinalizeHookNode* node = current;
        current = current->next;
        Shizu_State1_deallocate(state, node);
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
  }
}
