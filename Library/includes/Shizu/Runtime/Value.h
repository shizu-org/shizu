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

#if !defined(SHIZU_RUNTIME_VALUE_H_INCLUDED)
#define SHIZU_RUNTIME_VALUE_H_INCLUDED




// int32_t, INT32_MIN, INT32_MAX
#include <inttypes.h>

// bool, true, false
#include <stdbool.h>

// FLT_MAX
#include <float.h>

#include "Shizu/Runtime/Configure.h"

#define Shizu_Reference(T) T*

#define Shizu_MaybeReference(T) T*

typedef struct Shizu_State Shizu_State;




typedef bool Shizu_Boolean;

#define Shizu_Boolean_True (true)

#define Shizu_Boolean_False (false)




typedef void (Shizu_CxxFunction)(Shizu_State* state);




typedef float Shizu_Float32;

#define Shizu_Float32_Minimum (-FLT_MAX)

#define Shizu_Float32_Maximum (+FLT_MAX)




typedef int32_t Shizu_Integer32;

#define Shizu_Integer32_Minimum (INT32_MIN)

#define Shizu_Integer32_Maximum (INT32_MAX)




typedef struct Shizu_List Shizu_List;




typedef struct Shizu_Map Shizu_Map;




typedef struct Shizu_Object Shizu_Object;




typedef struct Shizu_String Shizu_String;




typedef uint8_t Shizu_Void;

#define Shizu_Void_Void (0)




typedef struct Shizu_WeakReference Shizu_WeakReference;




typedef struct Shizu_Value Shizu_Value;

#define Shizu_Value_Tag_Boolean (0)

#define Shizu_Value_Tag_CxxFunction (1)

#define Shizu_Value_Tag_Float32 (2)

#define Shizu_Value_Tag_Integer32 (3)

#define Shizu_Value_Tag_Object (6)

#define Shizu_Value_Tag_Void (8)

struct Shizu_Value {
  uint8_t tag;
  union {
    Shizu_Boolean booleanValue;
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue;
    Shizu_Float32 float32Value;
    Shizu_Integer32 integer32Value;
    Shizu_Object* objectValue;
    Shizu_Void voidValue;
  };
};

Shizu_Boolean
Shizu_Value_getBoolean
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isBoolean
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setBoolean
  (
    Shizu_Value* self,
    Shizu_Boolean booleanValue
  );

Shizu_Reference(Shizu_CxxFunction)
Shizu_Value_getCxxFunction
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isCxxFunction
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setCxxFunction
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue
  );

Shizu_Float32
Shizu_Value_getFloat32
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isFloat32
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setFloat32
  (
    Shizu_Value* self,
    Shizu_Float32 float32Value
  );

Shizu_Integer32
Shizu_Value_getInteger32
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isInteger32
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setInteger32
  (
    Shizu_Value* self,
    Shizu_Integer32 integer32Value
  );

Shizu_Reference(Shizu_Object)
Shizu_Value_getObject
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isObject
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setObject
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_Object) objectValue
  );

Shizu_Void
Shizu_Value_getVoid
  (
    Shizu_Value const* self
  );

bool
Shizu_Value_isVoid
  (
    Shizu_Value const* self
  );

void
Shizu_Value_setVoid
  (
    Shizu_Value* self,
    Shizu_Void voidValue
  );



#endif // SHIZU_RUNTIME_VALUE_H_INCLUDED
