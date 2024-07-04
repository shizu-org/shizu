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

#if !defined(SHIZU_RUNTIME_STACK_H_INCLUDED)
#define SHIZU_RUNTIME_STACK_H_INCLUDED



#include "Shizu/Runtime/Value.h"
// size_t
#include <stddef.h>
typedef struct Shizu_State1 Shizu_State1;



typedef struct Shizu_Stack Shizu_Stack;

size_t
Shizu_Stack_getSize
  (
    Shizu_State1* state,
    Shizu_Stack* self
  );

void
Shizu_Stack_push
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Value value
  );

void
Shizu_Stack_pop
  (
    Shizu_State1* state,
    Shizu_Stack* self
  );

Shizu_Value
Shizu_Stack_peek
  (
    Shizu_State1* state,
    Shizu_Stack* self
  );

void
Shizu_Stack_clear
  (
    Shizu_State1* state,
    Shizu_Stack* self
  );



static inline void
Shizu_Stack_pushBoolean
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Boolean booleanValue
  )
{
  Shizu_Value value;
  Shizu_Value_setBoolean(&value, booleanValue);
  Shizu_Stack_push(state, self, value);
}

static inline void
Shizu_Stack_pushCxxFunction
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_CxxFunction* cxxFunctionValue
  )
{
  Shizu_Value value;
  Shizu_Value_setCxxFunction(&value, cxxFunctionValue);
  Shizu_Stack_push(state, self, value);
}

static inline void
Shizu_Stack_pushInteger32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Integer32 integer32Value
  )
{
  Shizu_Value value;
  Shizu_Value_setInteger32(&value, integer32Value);
  Shizu_Stack_push(state, self, value);
}

static inline void
Shizu_Stack_pushFloat32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Float32 float32Value
  )
{
  Shizu_Value value;
  Shizu_Value_setFloat32(&value, float32Value);
  Shizu_Stack_push(state, self, value);
}

static inline void
Shizu_Stack_pushObject
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Object* objectValue
  )
{
  Shizu_Value value;
  Shizu_Value_setObject(&value, objectValue);
  Shizu_Stack_push(state, self, value);
}

static inline void
Shizu_Stack_pushVoid
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    Shizu_Void voidValue
  )
{
  Shizu_Value value;
  Shizu_Value_setVoid(&value, voidValue);
  Shizu_Stack_push(state, self, value);
}

bool
Shizu_Stack_isBoolean
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isCxxFunction
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isFloat32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isInteger32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isObject
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isType
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

bool
Shizu_Stack_isVoid
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Boolean
Shizu_Stack_getBoolean
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_CxxFunction*
Shizu_Stack_getCxxFunction
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Float32
Shizu_Stack_getFloat32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Integer32
Shizu_Stack_getInteger32
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Object*
Shizu_Stack_getObject
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Type*
Shizu_Stack_getType
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

Shizu_Void
Shizu_Stack_getVoid
  (
    Shizu_State1* state,
    Shizu_Stack* self,
    size_t index
  );

#endif // SHIZU_RUNTIME_STACK_H_INCLUDED
