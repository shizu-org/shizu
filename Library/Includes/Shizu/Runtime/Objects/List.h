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

#if !defined(SHIZU_RUNTIME_LIST_H_INCLUDED)
#define SHIZU_RUNTIME_LIST_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
	#error("Do not include `Shizu/Runtime/Objects/List.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Value.h"

Shizu_declareType(Shizu_List);

/// @since 1.0
/// @brief Create a Shizu_List value.
/// @param state A pointer to a Shizu_State value.
/// @return A pointer to the Shizu_List value.
/// @undefined state does not point to a Shizu_State value.
Shizu_List*
Shizu_List_create
  (
    Shizu_State* state
  );

Shizu_Value
Shizu_List_getValue
	(
		Shizu_State* state,
		Shizu_List* self,
		size_t index
	);

Shizu_Value
Shizu_List_getSize
	(
		Shizu_State* state,
		Shizu_List* self
	);

void
Shizu_List_insertValue
	(
		Shizu_State* state,
		Shizu_List* self,
		size_t index,
		Shizu_Value const* value
	);

void
Shizu_List_appendValue
	(
		Shizu_State* state,
		Shizu_List* self,
		Shizu_Value const* value
	);

void
Shizu_List_prependValue
	( 
		Shizu_State* state,
		Shizu_List* self,
		Shizu_Value const* value
	);

/**
 * @since 0.1
 * @brief Append a Shizu_Boolean object to a Shizu_List object.
 * @param self A pointer to the Shizu_List object to append to.
 * @param element A Shizu_Boolean object to append.
 * @undefined state does not point to a Shizu_State object.
 * @undefined self does not point to a Shizu_List object.
 */
static inline void
Shizu_List_appendBoolean
	(
		Shizu_State* state,
		Shizu_List* list,
		Shizu_Boolean element
	)
{
	Shizu_Value value;
	Shizu_Value_setBoolean(&value, element);
	Shizu_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Shizu_CxxFunction object to a Shizu_List object.
 * @param self A pointer to the Shizu_List object to append to.
 * @param element A pointer to the Shizu_CxxFunction object to append.
 * @undefined state does not point to a Shizu_State object.
 * @undefined self does not point to a Shizu_List object.
 * @undefined element does not point to a Shizu_CxxnFunction object.
 */
static inline void
Shizu_List_appendCxxFunction
	(
		Shizu_State* state,
		Shizu_List* list,
		Shizu_CxxFunction* element
	)
{
	Shizu_Value value;
	Shizu_Value_setCxxFunction(&value, element);
	Shizu_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Shizu_Integer32 object to a Shizu_List object.
 * @param self A pointer to the Shizu_List object to append to.
 * @param element the Shizu_Integer32 object to append.
 * @undefined state does not point to a Shizu_State object.
 * @undefined self does not point to a Shizu_List object.
 */
static inline void
Shizu_List_appendInteger32
	(
		Shizu_State* state,
		Shizu_List* list,
		Shizu_Integer32 element
	)
{
	Shizu_Value value;
	Shizu_Value_setInteger32(&value, element);
	Shizu_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Shizu_Object* object to a Shizu_List object.
 * @param self A pointer to the Shizu_List object to append to.
 * @param element A pointer to the Shizu_Object object to append.
 * @undefined state does not point to a Shizu_State object.
 * @undefined self does not point to a Shizu_List object.
 * @undefined element is not a Shizu_Object object.
 */
static inline void
Shizu_List_appendObject
	(
		Shizu_State* state,
		Shizu_List* list,
		Shizu_Object* element
	)
{
	Shizu_Value value;
	Shizu_Value_setObject(&value, element);
	Shizu_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Shizu_Void object to a Shizu_List object.
 * @param self A pointer to the Shizu_List object to append to.
 * @param element The Shizu_Void object to append.
 * @undefined state does not point to a Shizu_State object.
 * @undefined self does not point to a Shizu_List object.
 * @warning Adding a Shizu_Void value is a no operation; nothing will be added.
 */
static inline void
Shizu_List_appendVoid
	(
		Shizu_State* state,
		Shizu_List* self,
		Shizu_Void element
	)
{
	Shizu_Value value;
	Shizu_Value_setVoid(&value, element);
	Shizu_List_appendValue(state, self, &value);
}

#endif // SHIZU_RUNTIME_LIST_H_INCLUDED
