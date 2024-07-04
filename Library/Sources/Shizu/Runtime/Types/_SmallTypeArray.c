#include "Shizu/Runtime/Types/_SmallTypeArray.h"

// malloc, realloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// uint8_t
#include <inttypes.h>

int
SmallTypeArray_initialize
  (
    SmallTypeArray* x
  )
{
  x->elements = malloc(sizeof(Shizu_Type*) * 1);
  if (!x->elements) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type*) * 1);
    return 1;
  }
  for (size_t i = 0, n = 1; i < n; ++i) {
    x->elements[i] = NULL;
  }
  x->capacity = 1;
  return 0;
}

void
SmallTypeArray_uninitialize
  (
    SmallTypeArray* x
  )
{
  free(x->elements);
  x->elements = NULL;
}

int
SmallTypeArray_append
  (
    SmallTypeArray* x,
    Shizu_Type* type
  )
{
  uint8_t n = SmallTypeArray_getSize(x);
  if (x->capacity == n) {
    if (x->capacity == 128) {
      return 1;
    }
    uint8_t oldCapacity = x->capacity;
    // This can never fail as the sequence is 1, 2, 4, 8, 16, ..., 128.
    // And if oldCapacity is 128, this point is never reached.
    uint8_t newCapacity = oldCapacity * 2;
    Shizu_Type** newElements = realloc(x->elements, sizeof(Shizu_Type*) * newCapacity);
    if (!newElements) {
      fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type*) * newCapacity);
      return 1;
    }
    for (size_t i = n; i < newCapacity; ++i) {
      newElements[i] = NULL;
    }
    x->capacity = newCapacity;
    x->elements = newElements;
  }
  x->elements[n] = type;
  return 0;
}

uint8_t
SmallTypeArray_getSize
  (
    SmallTypeArray* x
  )
{
  uint8_t size = 0;
  for (size_t i = 0, n = x->capacity; i < n; ++i) {
    if (!x->elements[i]) {
      break;
    } else {
      size++;
    }
  }
  return size;
}

void
SmallTypeArray_ensureRemoved
  (
    SmallTypeArray* x,
    Shizu_Type* type
  )
{
  uint8_t index1 = 255;
  for (uint8_t i = 0, n = x->capacity; i < n; ++i) {
    if (x->elements[i] == type) {
      index1 = i;
      break;
    }
  }
  uint8_t index2 = (size_t)255;
  for (uint8_t i = x->capacity; i > 0; --i) {
    if (x->elements[i - 1]) {
      index2 = i - 1;
      break;
    }
  }
  x->elements[index1] = x->elements[index2];
  x->elements[index2] = NULL;
}
