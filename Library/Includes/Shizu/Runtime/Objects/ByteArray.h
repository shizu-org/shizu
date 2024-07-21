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

#if !defined(SHIZU_RUNTIME_OBJECTS_BYTEARRAY_H_INCLUDED)
#define SHIZU_RUNTIME_OBJECTS_BYTEARRAY_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Objects/ByteArray.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/Object.h"

/// @brief
/// The Machine Language type is
/// @code
/// class Shizu.ByteArray
/// @endcode
/// @remarks
/// Shizu_ByteArray is used to transfer Bytes between the managed and the environment.
Shizu_declareObjectType(Shizu_ByteArray);

/// @since 1.0
/// @brief Create a Shizu_ByteArray value.
/// @param state A pointer to a Shizu_State2 value.
/// @return A pointer to the Shizu_ByteArray value.
/// @undefined state does not point to a Shizu_State value.
Shizu_ByteArray*
Shizu_ByteArray_create
  (
    Shizu_State2* state
  );

Shizu_Value
Shizu_ByteArray_getValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t index
  );

Shizu_Value
Shizu_ByteArray_getSize
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  );

void
Shizu_ByteArray_insertValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t index,
    Shizu_Value const* value
  );

void
Shizu_ByteArray_appendValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Value const* value
  );

void
Shizu_ByteArray_prependValue
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Value const* value
  );

void
Shizu_ByteArray_insertRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    size_t i,
    void const* p,
    size_t n
  );

void
Shizu_ByteArray_prependRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    void const* p,
    size_t n
  );

void
Shizu_ByteArray_appendRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    void const* p,
    size_t n
  );

void*
Shizu_ByteArray_getRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  );

size_t
Shizu_ByteArray_getNumberOfRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray* self
  );

Shizu_Boolean
Shizu_ByteArray_compareRawBytes
  (
    Shizu_State2* state,
    Shizu_ByteArray const* self,
    Shizu_ByteArray const* other
  );


void
Shizu_ByteArray_clear
  (
    Shizu_State2* state,
    Shizu_ByteArray* self    
  );

// Remove count Bytes from the front of the Byte array.
// Raise an error if count is greater than the size of the Byte array.
// Raise an error if count is negative.
void
Shizu_ByteArray_removeFront
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Integer32 count
  );

// Remove count Bytes from the back of the Byte array.
// Raise an error if count is greater than the size of the Byte array.
// Raise an error if count is negative.
void
Shizu_ByteArray_removeBack
  (
    Shizu_State2* state,
    Shizu_ByteArray* self,
    Shizu_Integer32 count
  );

/// Remove leading occurrences of v.
void
Shizu_ByteArray_trimLeading
  (
    Shizu_State2* state,
    Shizu_ByteArray* a,
    Shizu_Integer32 v
  );

/// Remove trailing occurrences of v.
void
Shizu_ByteArray_trimTrailing
  (
    Shizu_State2* state,
    Shizu_ByteArray* a,
    Shizu_Integer32 v
  );

void
Shizu_ByteArray_reverse
  (
    Shizu_State2* state,
    Shizu_ByteArray* a
  );

#endif // SHIZU_RUNTIME_OBJECTS_BYTEARRAY_H_INCLUDED
