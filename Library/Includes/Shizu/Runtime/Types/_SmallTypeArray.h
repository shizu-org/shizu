#if !defined(SHIZU_RUNTIME_TYPES__SMALLTYPEARRAY_H_INCLUDED)
#define SHIZU_RUNTIME_TYPES__SMALLTYPEARRAY_H_INCLUDED

// uint8_t
#include <inttypes.h>

typedef struct Shizu_Type Shizu_Type;

// SmallTypeArray.capacity is the size of the array pointed to by SmallTypeArray.elements.
// The first n <= SmallTypeArray.capacity entries point to elements, the remaining elements are NULL.
typedef struct SmallTypeArray {
  Shizu_Type** elements;
  uint8_t capacity;
} SmallTypeArray;

// undefined if @a x does not point to an uninitialized small type array
int
SmallTypeArray_initialize
  (
    SmallTypeArray* x
  );

// undefined if @a x does not point to an initialized small type array
void
SmallTypeArray_uninitialize
  (
    SmallTypeArray* x
  );

// undefined if @a x does not point to a small type array
// undefined if @a type does not point to a type
int
SmallTypeArray_append
  (
    SmallTypeArray* x,
    Shizu_Type* type
  );

// undefined if @a x does not point to a small type array
// this function always succeeds
uint8_t
SmallTypeArray_getSize
  (
    SmallTypeArray* x
  );

// undefined if @a x does not point to a small type array
// undefined if @a type does not point to a type
void
SmallTypeArray_ensureRemoved
  (
    SmallTypeArray* x,
    Shizu_Type* type
  );

#endif // SHIZU_RUNTIME_TYPES__SMALLTYPEARRAY_H_INCLUDED
