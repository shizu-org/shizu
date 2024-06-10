#if !defined(SHIZU_RUNTIME_OBJECT_H_INCLUDED)
#define SHIZU_RUNTIME_OBJECT_H_INCLUDED

#include "Shizu/Runtime/NoReturn.h"
#include "Shizu/Runtime/Type.h"
#include "Shizu/Runtime/Value.h"
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Object_Dispatch Shizu_Object_Dispatch;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_State Shizu_State;

/// @brief Get the Shizu_Type value of a Shizu_Object value.
/// @param self A pointer to the Shizu_State object.
/// @param object A pointer the Shizu_Object value.
/// @return A pointer to the Shizu_Type value of the Shizu_Object value.
Shizu_Type*
Shizu_State_getObjectType
  (
    Shizu_State* self,
    Shizu_Object* object
  );

/// @brief Get the Shizu_Object_Dispatch value of a Shizu_Object value.
/// @param self A pointer to the Shizu_State object.
/// @param object A pointer the Shizu_Object value.
/// @return A pointer to the Shizu_Object_dispatch value of the Shizu_Object value.
Shizu_Object_Dispatch*
Shizu_State_getObjectDispatch
  (
    Shizu_State* state,
    Shizu_Object* object
  );

Shizu_NoReturn() void
Shizu_Errors_raiseDispatchNotExists
  (
    Shizu_State* state,
    Shizu_Object* target,
    char const* methodName,
    size_t methodNameLength
  );

Shizu_NoReturn() void
Shizu_Errors_raiseMethodNotImplemented
  (
    Shizu_State* state,
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
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    Shizu_Errors_raiseDispatchNotExists(state, self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  if (!dispatch->METHOD) { \
    Shizu_Errors_raiseMethodNotImplemented(state, self, #METHOD, sizeof(#METHOD) - 1); \
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
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    Shizu_Errors_raiseDispatchNotExists(state, self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  if (!dispatch->METHOD) { \
    Shizu_Errors_raiseMethodNotImplemented(state, self, #METHOD, sizeof(#METHOD) - 1); \
  } \
  return dispatch->METHOD(state, __VA_ARGS__);

Shizu_declareType(Shizu_Object);

struct Shizu_Object_Dispatch {
  Shizu_Object_Dispatch* parent;
  Shizu_Integer32(*getHashValue)(Shizu_State* state, Shizu_Object* self);
  Shizu_Boolean(*isEqualTo)(Shizu_State* state, Shizu_Object* self, Shizu_Object* other);
};

struct Shizu_Object {
  // Shizu_Object are kept in a singly-linked list.
  Shizu_Object* next;
  // Shizu_Object values are kept in the singly-linked "gray" list.
  Shizu_Object* gray;
  // Can assume the following values;
  // - 1 (decimal) or 01 (binary).
  // Indicates the color WHITE.
  // - 2 (decimal) or 10 (binary).
  // Indicates the color BLACK.
  // - 3 (decimal) or 01 | 10 (binary).
  // Indicates the color GRAY.
  uint8_t flags;
  // Pointer to the Shizu_Type object.
  Shizu_Type* type;
};

/// @brief Construct this Shizu_Object value.
/// @param state A pointer to the Shizu_State object.
/// @param self A pointer to this Shizu_Object.
void
Shizu_Object_construct
  (
    Shizu_State* state,
    Shizu_Object* self
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
static inline Shizu_Integer32
Shizu_Object_getHashValue
  (
    Shizu_State* state,
    Shizu_Object* self
  )
{ Shizu_VirtualCallWithReturn(Shizu_Object, getHashValue, self); }

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
static inline Shizu_Boolean
Shizu_Object_isEqualTo
  (
    Shizu_State* state,
    Shizu_Object* self,
    Shizu_Object* other
  )
{ Shizu_VirtualCallWithReturn(Shizu_Object, isEqualTo, self, other); }

#endif // SHIZU_RUNTIME_OBJECT_H_INCLUDED
