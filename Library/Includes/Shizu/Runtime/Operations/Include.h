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

#if !defined(SHIZU_RUNTIME_OPERATIONS_INCLUDE_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_INCLUDE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Operations/Include.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif

#include "Shizu/Runtime/Value.h"

/// Get the type of the specified name.
/// @code
/// returnValue <~ _getType(<name>)
/// _typeOf(returnValue) = Type
/// returnValue.name = <name>
/// @endcode
void
Shizu_Operations_getType
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _typeOf(argumentValue[0])
/// @endcode
void
Shizu_Operations_typeOf
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _create(argumentValue[0], argumentValue[1], ..., argumentValue[n-1])
/// n > 0, _typeOf(argumentValue[0]) = Type
/// @endcode
void
Shizu_Operations_create
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <- _not(argumentValue[0])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 1, typeOf(argumentValue[0]) = Boolean
/// @endcode
void
Shizu_Operations_not
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );


/// @code
/// returnValue <- _and(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Boolean, i in {0,1}
/// @endcode
void
Shizu_Operations_and
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <- _or(argumentValue[0], argumentValue[1])
/// n = 2, _typeOf(argumentValue[0]) = Boolean
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Boolean, i in {0,1}
/// @endcode
void
Shizu_Operations_or
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <- _add_i32(argumentValue[0], argumentValue[1])
/// n = 2, _typeOf(argumentValue[i]) = Integer32, i in {0,1}
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Integer32, i in {0,1}
/// @endcode
void
Shizu_Operations_add_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _add_f32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float32, i in {0,1}
/// @endcode
void
Shizu_Operations_add_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#if 1 == Shizu_Configuration_WithFloat64

/// @code
/// returnValue <~ _add_f64(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float64, i in {0,1}
/// @endcode
void
Shizu_Operations_add_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#endif

/// @code
/// returnValue <~ _subtract_i32(argumentValue[0], argumentValue[1])
/// n = 2, TYPEOF(argumentValue[0]) = Integer32
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Integer32, i in {0,1}
/// @endcode
void
Shizu_Operations_subtract_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _subtract_f32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float32, i in {0,1}
/// @endcode
void
Shizu_Operations_subtract_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#if 1 == Shizu_Configuration_WithFloat64

/// @code
/// returnValue <~ _subtract_f64(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float64, i in {0,1}
/// @endcode
void
Shizu_Operations_subtract_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#endif

/// @code
/// returnValue <- _multiply_i32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Integer32, i in {0,1}
/// @endcode
void
Shizu_Operations_multiply_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _multiply_f32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float32, i in {0,1}
/// @endcode
void
Shizu_Operations_multiply_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#if 1 == Shizu_Configuration_WithFloat64

/// @code
/// returnValue <~ _multiply_f64(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float64, i in {0,1}
/// @endcode
void
Shizu_Operations_multiply_f64
  (
    Shizu_State2 * state,
    Shizu_Value * returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value * argumentValues
  );

#endif

/// @code
/// returnValue <~ _divide_i32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Integer32, i in {0,1}
/// @endcode
void
Shizu_Operations_divide_i32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

/// @code
/// returnValue <~ _divide_f32(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float32, i in {0,1}
/// @endcode
void
Shizu_Operations_divide_f32
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#if 1 == Shizu_Configuration_WithFloat64

/// @code
/// returnValue <~ _divide_f64(argumentValue[0], argumentValue[1])
/// @endcode
/// with preconditions
/// @code
/// numberOfArgumentValues = 2, _typeOf(argumentValue[i]) = Float64, i in {0,1}
/// @endcode
void
Shizu_Operations_divide_f64
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

#endif

#endif // SHIZU_RUNTIME_OPERATIONS_INCLUDE_H_INCLUDED
