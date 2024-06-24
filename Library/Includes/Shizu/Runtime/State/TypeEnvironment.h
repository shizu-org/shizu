#if !defined(SHIZU_RUNTIME_STATE_TYPEENVIRONMENT_H_INCLUDED)
#define SHIZU_RUNTIME_STATE_TYPEENVIRONMENT_H_INCLUDED

#include "Shizu/Runtime/Objects/Environment.h"
#include "Shizu/Runtime/State2.h"

typedef struct _TypeEnvironmentNode _TypeEnvironmentNode;
typedef struct _TypeEnvironments _TypeEnvironments;

struct _TypeEnvironmentNode {
  /// Null pointer or the successor in a singly-linked list.
  _TypeEnvironmentNode* next;
  /// Pointer to the type.
  Shizu_Type* type;
  /// Pointer to the environment.
  Shizu_Environment* environment;
};

typedef struct _TypeEnvironments {
  _TypeEnvironmentNode** buckets;
  size_t size;
  size_t capacity;
} _TypeEnvironments;

static inline void
_TypeEnvironments_initialize
  (
    Shizu_State1* state,
    _TypeEnvironments* typeEnvironments
  )
{
  _TypeEnvironmentNode**buckets = Shizu_State1_allocate(state, sizeof(_TypeEnvironmentNode*) * 8);
  if (!buckets) {
    Shizu_State1_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State1_jump(state);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    buckets[i] = NULL;
  }
  typeEnvironments->size = 0;
  typeEnvironments->capacity = 8;
}

static inline void
_TypeEnvironments_uninitialize
  (
    Shizu_State1* state,
    _TypeEnvironments* typeEnvironments
  )
{                                  
  if (typeEnvironments->buckets) {
    for (size_t i = 0, n = typeEnvironments->capacity; i < n; ++i) {
      while (typeEnvironments->buckets[i]) {
        _TypeEnvironmentNode* node = typeEnvironments->buckets[i];
        typeEnvironments->buckets[i] = node->next;
        Shizu_State1_deallocate(state, node);
      }
    }
    Shizu_State1_deallocate(state, typeEnvironments->buckets);
    typeEnvironments->buckets = NULL;
  }
}

/// Get the type environment of a type.
/// The environment is created if it does not exist and @a create is @a true.
/// Return the environment if it exists and return a null pointer otherwise. 
/// If it does not exist, return NULL.
static inline Shizu_Environment*
_TpeEnvironments_getTypeEnvironment
  (
    Shizu_State2* state,
    Shizu_Type* type,
    Shizu_Boolean create
  )
{ }

#endif // SHIZU_RUNTIME_STATE_TYPEENVIRONMENT_H_INCLUDED
