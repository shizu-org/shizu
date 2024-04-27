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
#include "Shizu/Runtime/Locks.private.h"
#include "Shizu/Runtime/Stack.private.h"
#include "Shizu/Runtime/Type.private.h"
#include "Shizu/Runtime/Objects/WeakReference.private.h"
#include "Shizu/Runtime/DebugAssert.h"
#include "Shizu/Runtime/staticAssert.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#define Shizu_Object_Flags_White (1)

#define Shizu_Object_Flags_Black (2)

#define Shizu_Object_Flags_Gray (Shizu_Object_Flags_White|Shizu_Object_Flags_Black)

#define Shizu_Gc_MarkStack_Capacity (1024)

typedef struct Gc {
  int referenceCount;
  Shizu_Object* all;
  Shizu_Object* gray;
} Gc;

Shizu_Gc*
Shizu_Gc_startup
  (
    Shizu_State* state
  )
{
  Shizu_Gc* self = malloc(sizeof(Gc));
  if (!self) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  self->all = NULL;
  self->gray = NULL;

  return self;
}

void
Shizu_Gc_shutdown
  (
    Shizu_State* state,
    Shizu_Gc* gc
  )
{
  free(gc);
  gc = NULL;
}

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  );

Shizu_TypeDescriptor const Shizu_Object_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Shizu_Object),
  .visit = NULL,
  .finalize = NULL,
  .dispatchSize = sizeof(Shizu_Object_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

static void
Shizu_Object_typeDestroyed
  (
    Shizu_State1* state1
  )
{/*Intentionally empty.*/}

Shizu_Type*
Shizu_Object_getType
  (
    Shizu_State* state
  )
{
  Shizu_Type* type = Shizu_State_getTypeByName(state, "Shizu_Object");
  if (!type) {
    type = Shizu_State_createType(state, "Shizu_Object", NULL, NULL, &Shizu_Object_typeDestroyed, &Shizu_Object_Type);
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
Shizu_Gc_allocate
  (
    Shizu_State* state,
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
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Object* self = malloc(size);
  if (!self) {
    fprintf(stderr, "%s:%d: unable to allocate `%zu` Bytes\n", __FILE__, __LINE__, size);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  Shizu_Gc* gc = Shizu_State_getGc(state);
  self->gray = NULL;
  self->flags = Shizu_Object_Flags_White;
  self->type = Shizu_Object_getType(state);
  self->next = gc->all;
  gc->all = self;
  return self;
}

void
Shizu_Gc_run
  (
    Shizu_State* state
  )
{
  Shizu_Gc* gc = Shizu_State_getGc(state);
  Shizu_debugAssert(NULL == gc->gray);
  // Premark.
  Shizu_Locks_notifyPreMark(state);
  Shizu_Stack_notifyPreMark(state);
  // Mark.
  while (gc->gray) {
    Shizu_Object* object = gc->gray;
    gc->gray = object->gray;
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
  Shizu_Object** previous = &gc->all;
  Shizu_Object* current = gc->all;
  while (NULL != current) {
    if (Shizu_Object_isWhite(current)) {
      Shizu_Object* object = current;
      *previous = current->next;
      current = current->next;
      // TODO: Shizu_Locks_notifyDestroy as well as Shizu_WeakReferences_notifyDestroy perform an object address hash lookup.
      //       Investigage if there is a relevant performance gain when only one hash table is used?
      Shizu_Locks_notifyDestroy(state, object);
      Shizu_WeakReferences_notifyDestroy(state, object);
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
  fprintf(stdout, "%s:%d: live = %zu, dead = %zu\n", __FILE__, __LINE__, live, dead);
}

void
Shizu_Gc_visitObject
  (
    Shizu_State* state,
    Shizu_Object* object
  )
{
  if (Shizu_Object_isWhite(object)) {
    if (object->type->descriptor->visit) {
      // If there is a visit function, put the object in the gray list and color it gray.
      Shizu_Object_setGray(object);
      Shizu_debugAssert(NULL == object->gray);
      Shizu_Gc* gc = Shizu_State_getGc(state);
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
    Shizu_State* state,
    Shizu_Value* value
  )
{
  switch (value->tag) {
    case Shizu_Value_Tag_Object: {
      Shizu_Gc_visitObject(state, (Shizu_Object*)Shizu_Value_getObject(value));
    } break;
    default: {
      /* Intentionally empty. */
    } break;
  }
}
