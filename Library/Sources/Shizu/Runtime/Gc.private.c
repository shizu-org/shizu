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
#include "Shizu/Runtime/Gc.private.h"

#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Object.h"
#include "Shizu/Runtime/Type.private.h"
#include "Shizu/Runtime/CxxUtilities.h"

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#define Shizu_Object_Flags_White (1)

#define Shizu_Object_Flags_Black (2)

#define Shizu_Object_Flags_Gray (Shizu_Object_Flags_White|Shizu_Object_Flags_Black)

Shizu_Gc*
Shizu_Gc_create
  (
    Shizu_State1* state1
  )
{
  Shizu_Gc* self = Shizu_State1_allocate(state1, sizeof(Shizu_Gc));
  if (!self) {
    Shizu_State1_setStatus(state1, 1);
    Shizu_State1_jump(state1);
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
    Shizu_State1* state1,
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
      Shizu_State1_deallocate(state1, node);
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
      Shizu_State1_deallocate(state1, node);
    } while (self->objectFinalizeHooks.nodes);
    if (live) {
      fprintf(stderr, "%s: %d: warning: object finalize hook node list not empty\n", __FILE__, __LINE__);
    }
  }
  Shizu_State1_deallocate(state1, self);
  self = NULL;
}

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  );

static Shizu_Integer32
Shizu_Object_getHashValueImpl
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

static Shizu_Boolean
Shizu_Object_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Object* other
  );

static void
Shizu_Object_initializeDispatch
  (
    Shizu_State1* state1,
    Shizu_Object_Dispatch* self
  );

Shizu_TypeDescriptor const Shizu_Object_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Shizu_Object),
  .visit = NULL,
  .finalize = NULL,
  .dispatchSize = sizeof(Shizu_Object_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Shizu_Object_initializeDispatch,
  .dispatchUninitialize = NULL,
};

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  )
{/*Intentionally empty.*/}

static Shizu_Integer32
Shizu_Object_getHashValueImpl
  ( 
    Shizu_State2* state,
    Shizu_Object* self
  )
{ return (Shizu_Integer32)(intptr_t)self; }

static Shizu_Boolean
Shizu_Object_isEqualToImpl
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Object* other
  )
{ return self == other; }

static void
Shizu_Object_initializeDispatch
  (
    Shizu_State1* state1,
    Shizu_Object_Dispatch* self
  )
{
  self->getHashValue = &Shizu_Object_getHashValueImpl;
  self->isEqualTo = &Shizu_Object_isEqualToImpl;
}

Shizu_Type*
Shizu_Object_getType
  (
    Shizu_State2* state
  )
{
  Shizu_Type* type = Shizu_Types_getTypeByName(Shizu_State2_getState1(state),
                                               Shizu_State2_getTypes(state),
                                               "Shizu_Object",
                                               sizeof("Shizu_Object") - 1);
  if (!type) {
    type = Shizu_Types_createType(Shizu_State2_getState1(state),
                                  Shizu_State2_getTypes(state),
                                  "Shizu_Object",
                                  sizeof("Shizu_Object") - 1,
                                  NULL,
                                  NULL,
                                  &Shizu_Object_typeDestroyed,
                                  &Shizu_Object_Type);
  }
  return type;
}

void
Shizu_Object_setWhite
  (
    Shizu_Object* object
  )
{ object->flags = (object->flags & ~Shizu_Object_Flags_Gray) | Shizu_Object_Flags_White; }

bool
Shizu_Object_isWhite
  (
    Shizu_Object const* object
  )
{ return Shizu_Object_Flags_White == (Shizu_Object_Flags_Gray & object->flags); }

void
Shizu_Object_setGray
  (
    Shizu_Object* object
  )
{ object->flags |= Shizu_Object_Flags_Gray; }

bool
Shizu_Object_isGray
  (
    Shizu_Object const* object
  )
{ return Shizu_Object_Flags_Gray == (Shizu_Object_Flags_Gray & object->flags); }

void
Shizu_Object_setBlack
  (
    Shizu_Object* object
  )
{ object->flags = (object->flags & ~Shizu_Object_Flags_Gray) | Shizu_Object_Flags_Black; }

bool
Shizu_Object_isBlack
  (
    Shizu_Object const* object
  )
{ return Shizu_Object_Flags_Black == (Shizu_Object_Flags_Gray & object->flags); }

Shizu_Object*
Shizu_Gc_allocateObject
  (
    Shizu_State2* state,
    size_t size
  )
{
#if Shizu_Configuration_InstructionSetArchitecture_X64 == Shizu_Configuration_InstructionSetArchitecture
  Shizu_staticAssert(sizeof(Shizu_Object) % 8 == 0, "`Shizu_Object` must be a multiple of 8");
#elif Shizu_Configuration_InstructionSetArchitecture_X86 == Shizu_Configuration_InstructionSetArchitecture
  Shizu_staticAssert(sizeof(Shizu_Object) % 4 == 0, "`Shizu_Object` must be a multiple of 4");
#else
  #error("instruction set architecture not (yet) supported")
#endif
  if (size < sizeof(Shizu_Object)) {
    fprintf(stderr, "%s:%d: size `%zu` is smaller than sizeof(Shizu_Object) = `%zu`\n", __FILE__, __LINE__, size, sizeof(Shizu_Object));
    Shizu_State1_setStatus(Shizu_State2_getState1(state), 1);
    Shizu_State1_jump(Shizu_State2_getState1(state));
  }
  Shizu_Object* self = Shizu_State1_allocate(Shizu_State2_getState1(state), size);
  if (!self) {
    fprintf(stderr, "%s:%d: unable to allocate `%zu` Bytes\n", __FILE__, __LINE__, size);
    Shizu_State1_setStatus(Shizu_State2_getState1(state), 1);
    Shizu_State1_jump(Shizu_State2_getState1(state));
  }
  Shizu_Gc* gc = Shizu_State2_getGc(state);
  self->gray = NULL;
  self->flags = Shizu_Object_Flags_White;
  self->type = Shizu_Object_getType(state);
  self->next = gc->all;
  gc->all = self;
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

void
Shizu_Gc_run
  (
    Shizu_State2* state,
    Shizu_Gc* self,
    Shizu_Gc_SweepInfo* sweepInfo
  )
{
  Shizu_debugAssert(NULL == self->gray); 
  notifyPreMarkHooks(state, self);
  // Mark.
  while (self->gray) {
    Shizu_Object* object = self->gray;
    self->gray = object->gray;
    object->gray = NULL;
    Shizu_debugAssert(!Shizu_Object_isWhite(object)); // Must not be white.
    Shizu_debugAssert(NULL == object->gray);
    if (!Shizu_Object_isBlack(object)) {
      Shizu_Object_setBlack(object);
      Shizu_debugAssert(NULL != object->type->descriptor->visit);
      object->type->descriptor->visit(state, object);
    }
  }
  // Sweep.
  size_t live = 0, dead = 0;
  Shizu_Object** previous = &self->all;
  Shizu_Object* current = self->all;
  while (NULL != current) {
    if (Shizu_Object_isWhite(current)) {
      Shizu_Object* object = current;
      *previous = current->next;
      current = current->next;
      // TODO: Shizu_Locks_notifyDestroy as well as Shizu_WeakReferences_notifyDestroy perform an object address hash lookup.
      //       Investigage if there is a relevant performance gain when only one hash table is used?
      notifyObjectFinalizeHooks(state, self, object);
      while (object->type) {
        if (object->type->descriptor->finalize) {
          object->type->descriptor->finalize(state, object);
        }
        object->type = object->type->parentType;
      }
      dead++;
      free(object);
    } else {
      Shizu_debugAssert(!Shizu_Object_isWhite(current));
      Shizu_Object_setWhite(current);
      previous = &current->next;
      current = current->next;
      live++;
    }
  }
  if (sweepInfo) {
    sweepInfo->live = live;
    sweepInfo->dead = dead;
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
  if (Shizu_Object_isWhite(object)) {
    if (object->type->descriptor->visit) {
      // If there is a visit function, put the object in the gray list and color it gray.
      Shizu_Object_setGray(object);
      Shizu_debugAssert(NULL == object->gray);
      object->gray = gc->gray;
      gc->gray = object;
    } else {
      // Otherwise color it black.
      Shizu_Object_setBlack(object);
    }
  }
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
