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
#include "Shizu/Runtime/Objects/String.private.h"

// malloc, free
#include <malloc.h>

// memcpy
#include <string.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// memcmp. memcpy
#include <string.h>

static void
Shizu_String_finalize
  (
    Shizu_State* state,
    Shizu_String* self
  );

static Shizu_Integer32
Shizu_String_getHashValueImpl
  (
    Shizu_State* state,
    Shizu_String* self
  );

static Shizu_Boolean
Shizu_String_isEqualToImpl
  (
    Shizu_State* state,
    Shizu_String* self,
    Shizu_Object* other
  );

static void
Shizu_String_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_String_Dispatch* self
  );

static Shizu_TypeDescriptor const Shizu_String_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Shizu_String),
  .visit = NULL,
  .finalize = (Shizu_OnFinalizeCallback*)&Shizu_String_finalize,
  .dispatchSize = sizeof(Shizu_String_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)Shizu_String_initializeDispatch,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_String, Shizu_Object);

static void
Shizu_String_finalize
  (
    Shizu_State* state,
    Shizu_String* self
  )
{
  if (self->bytes) {
    free(self->bytes);
    self->bytes = NULL;
  }
}

static Shizu_Integer32
Shizu_String_getHashValueImpl
  (
    Shizu_State* state,
    Shizu_String* self
  )
{
  return (Shizu_Integer32)self->hashValue;
}

static Shizu_Boolean
Shizu_String_isEqualToImpl
  (
    Shizu_State* state,
    Shizu_String* self,
    Shizu_Object* other
  )
{
  if (Shizu_State_isSubTypeOf(state, Shizu_String_getType(state), Shizu_State_getObjectType(state, other))) {
    Shizu_String* x = self;
    Shizu_String* y = (Shizu_String*)other;
    if (x->hashValue == y->hashValue && x->numberOfBytes == y->numberOfBytes) {
      return !memcmp(x->bytes, y->bytes, x->numberOfBytes);
    } else {
      return Shizu_Boolean_False;
    }
  } else {
    return Shizu_Boolean_False;
  }
}

static void
Shizu_String_initializeDispatch
  (
    Shizu_State1* state,
    Shizu_String_Dispatch* self
  )
{
  ((Shizu_Object_Dispatch*)self)->getHashValue = (Shizu_Integer32 (*)(Shizu_State*, Shizu_Object*)) & Shizu_String_getHashValueImpl;
  ((Shizu_Object_Dispatch*)self)->isEqualTo = (Shizu_Boolean (*)(Shizu_State*, Shizu_Object*, Shizu_Object*)) & Shizu_String_isEqualToImpl;
}

Shizu_String*
Shizu_String_create
  (
    Shizu_State* state,
    char const* bytes,
    size_t numberOfBytes
  )
{
  Shizu_Type* TYPE = Shizu_String_getType(state);
  Shizu_String* self = (Shizu_String*)Shizu_Gc_allocate(state, sizeof(Shizu_String));
  self->bytes = malloc(numberOfBytes > 0 ? numberOfBytes : 1);
  if (!self->bytes) {
    fprintf(stderr, "%s:%d: unable to allocate `%zu` Bytes\n", __FILE__, __LINE__, numberOfBytes > 0 ? numberOfBytes : 1);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = ((hashValue << 5) ^ (hashValue >> 3)) | (size_t)bytes[i];
  }
  self->hashValue = hashValue;
  self->numberOfBytes = numberOfBytes;
  memcpy(self->bytes, bytes, numberOfBytes);
  ((Shizu_Object*)self)->type = TYPE;
  return self;
}

Shizu_String*
Shizu_String_concatenate
	(
		Shizu_State* state,
		Shizu_String* self,
		Shizu_String* other
	)
{
	// The string would be too long.
	if (SIZE_MAX - self->numberOfBytes < other->numberOfBytes) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
	}
  size_t hashValue = self->numberOfBytes + other->numberOfBytes;
	for (size_t i = 0, n = self->numberOfBytes; i < n; ++i) {
		hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)self->bytes[i];
	}
	for (size_t i = 0, n = other->numberOfBytes; i < n; ++i) {
		hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)other->bytes[i];
	}
  Shizu_Type* TYPE = Shizu_String_getType(state);
  Shizu_String* new = (Shizu_String*)Shizu_Gc_allocate(state, sizeof(Shizu_String));
  size_t numberOfBytes = self->numberOfBytes + other->numberOfBytes;
  new->bytes = malloc(numberOfBytes > 0 ?  numberOfBytes : 1);
  if (!new->bytes) {
    fprintf(stderr, "%s:%d: unable to allocate `%zu` Bytes\n", __FILE__, __LINE__, numberOfBytes > 0 ? numberOfBytes : 1);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  new->hashValue = hashValue;
	new->numberOfBytes = numberOfBytes;
	memcpy(new->bytes, self->bytes, self->numberOfBytes);
	memcpy(new->bytes + self->numberOfBytes, other->bytes, other->numberOfBytes);
  ((Shizu_Object*)self)->type = TYPE;
	return new;
}

char const*
Shizu_String_getBytes
  (
    Shizu_State* state,
    Shizu_String* self
  )
{ return self->bytes; }

size_t
Shizu_String_getNumberOfBytes
  (
    Shizu_State* state,
    Shizu_String* self
  )
{ return self->numberOfBytes; }

Shizu_Boolean
Shizu_String_startsWith
  (
    Shizu_State* state,
    Shizu_String* self,
    Shizu_String* prefix
  )
{
  if (prefix->numberOfBytes > self->numberOfBytes) {
    return false;
  }
  return !memcmp(prefix->bytes, self->bytes, prefix->numberOfBytes);
}

Shizu_Boolean
Shizu_String_endsWith
  (
    Shizu_State* state,
    Shizu_String* self,
    Shizu_String* suffix
  )
{
  if (suffix->numberOfBytes > self->numberOfBytes) {
    return false;
  }
  size_t start = self->numberOfBytes - suffix->numberOfBytes;
  return !memcmp(suffix->bytes, self->bytes + start, suffix->numberOfBytes);
}
