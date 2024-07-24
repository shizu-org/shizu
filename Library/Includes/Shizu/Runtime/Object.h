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

#if !defined(SHIZU_RUNTIME_OBJECT_H_INCLUDED)
#define SHIZU_RUNTIME_OBJECT_H_INCLUDED

#include "Shizu/Runtime/Type.h"
#include "Shizu/Runtime/Value.h"
#include "Shizu/Runtime/CxxUtilities.h"
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Object_Dispatch Shizu_Object_Dispatch;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_State2 Shizu_State2;

Shizu_NoReturn() void
Shizu_Errors_raiseDispatchNotExists
  (
    Shizu_State2* state,
    Shizu_SourceLocationC sourceLocationC,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  );

Shizu_NoReturn() void
Shizu_Errors_raiseMethodNotImplemented
  (
    Shizu_State2* state,
    Shizu_SourceLocationC sourceLocationC,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  );

/// @brief Macro to facilate the definition of methods.
/// @code
/// static inline void
/// A_f
///   (
///     Shizu_State* state,
///     A* self,
///     Shizu_Integer32 x
///   )
/// { Shizu_VirtualCall(A, f, self, x); }
/// @endcode
#define Shizu_VirtualCall(TYPE, METHOD, ...) \
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_Object_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    Shizu_Errors_raiseDispatchNotExists(state, Shizu_SourceLocationC(), (Shizu_Object*)self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  if (!dispatch->METHOD) { \
    Shizu_Errors_raiseMethodNotImplemented(state, Shizu_SourceLocationC(), (Shizu_Object*)self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  dispatch->METHOD(state, __VA_ARGS__);

/// @brief Macro to facilate the definition of methods.
/// @code
/// static inline Shizu_Integer32
/// A_f
///   (
///     Shizu_State* state,
///     A* self,
///     Shizu_Integer32 x
///   )
/// { Shizu_VirtualCallWithReturn(A, f, self, x); }
/// @endcode
#define Shizu_VirtualCallWithReturn(TYPE, METHOD, ...) \
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_Object_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    Shizu_Errors_raiseDispatchNotExists(state, Shizu_SourceLocationC(), (Shizu_Object*)self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  if (!dispatch->METHOD) { \
    Shizu_Errors_raiseMethodNotImplemented(state, Shizu_SourceLocationC(), (Shizu_Object*)self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  return dispatch->METHOD(state, __VA_ARGS__);

Shizu_declareObjectType(Shizu_Object);

struct Shizu_Object_Dispatch {
  Shizu_Object_Dispatch* parent;
  void (*call)(Shizu_State2* state, Shizu_Object* self, uint8_t const* methodNameBytes, size_t numberOfMethodNameBytes, Shizu_Value* returnVallue, Shizu_Integer32 numberOfArguments, Shizu_Value* arguments);
  Shizu_Integer32(*getHashValue)(Shizu_State2* state, Shizu_Object* self);
  Shizu_Boolean(*isEqualTo)(Shizu_State2* state, Shizu_Object* self, Shizu_Value const* other);
};

struct Shizu_Object {
  // Pointer to the Shizu_Type object.
  Shizu_Type* type;
};

/// @brief Construct this Shizu_Object value.
/// @param state A pointer to a Shizu_State2 state.
/// @param self A pointer to this Shizu_Object.
void
Shizu_Object_construct
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

/// @ingroup Object
/// @brief
/// Call this Shizu_Object object.
/// @param state
/// A pointer to the Shizu_State object.
/// @param self
/// A pointer to this Shizu_Object object.
/// @param methodNameBytes, numberOfMethodNameBytes
/// UTF-8 method name of the method to call.
/// @param returnValue
/// A pointer to a Shizu_Value object receiving the return value.
/// @param numberOfArguments
/// The number of arguments.
/// @param arguments
/// A pointer to an array of @a numberOfArguments Shizu_Value objects denoting the arguments.
void
Shizu_Object_call
  (
    Shizu_State2* state,
    Shizu_Object* self,
    uint8_t const* methodNameBytes,
    size_t numberOfMethodNameBytes,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArguments,
    Shizu_Value* arguments
  );

/// @ingroup Object
/// @brief
/// Get the hash value of this Shizu_Object object.
/// @param state
/// A pointer to the Shizu_State object.
/// @param self
/// A pointer to this Shizu_Object object.
/// @return
/// The hash value of this Shizu_Object object.
Shizu_Integer32
Shizu_Object_getHashValue
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

/// @ingroup Object
/// @brief
/// Get if this Shizu_Object object is equal to another Shizu_Object object.
/// @param state
/// A pointer to the Shizu_State object.
/// @param self
/// A pointer to this Shizu_Object object.
/// @param other
/// A pointer to the other Shizu_Object object.
/// @return
/// @a true if this Shizu_Object ojbect is equal to the other Shizu_Object object.
/// @a false otherwise.
Shizu_Boolean
Shizu_Object_isEqualTo
  (
    Shizu_State2* state,
    Shizu_Object* self,
    Shizu_Value const* other
  );

/// @brief Get the Shizu_Type value of a Shizu_Object object.
/// @param state A pointer to the Shizu_State2 object.
/// @param self A pointer the Shizu_Object object.
/// @return A pointer to the Shizu_Type object of the Shizu_Object object.
/// @undefined @a state does not point to a Shizu_State2 object.
/// @undefined @a self does not point to a shizu_Object object.
Shizu_Type*
Shizu_Object_getObjectType
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

/// @brief Get the Shizu_Object_Dispatch value of a Shizu_Object value.
/// @param state A pointer to the Shizu_State object.
/// @param self A pointer the Shizu_Object value.
/// @return A pointer to the Shizu_Object_dispatch value of the Shizu_Object value.
Shizu_Object_Dispatch*
Shizu_Object_getObjectDispatch
  (
    Shizu_State2* state,
    Shizu_Object* self
  );

#endif // SHIZU_RUNTIME_OBJECT_H_INCLUDED
