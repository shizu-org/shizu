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

#if !defined(SHIZU_RUNTIME_STATICASSERT_H_INCLUDED)
#define SHIZU_RUNTIME_STATICASSERT_H_INCLUDED

#include "Shizu/Runtime/Configure.h"

/// @since 1.0
/// @brief Macro aliasing `static_assert`/`_Static_assert`.
/// @details If the expression @a expression evaluates to logically false then a compilation error is generated with the message @a message.
/// @param expression A compile-time evaluable expression that can be converted into logically true or logically false.
/// @param message A string literal.
#if Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC

  #define Shizu_staticAssert(expression, message) \
    static_assert(expression, message);

#else

  #define Shizu_staticAssert(expression, message) \
    _Static_assert(expression, message);

#endif

#endif // SHIZU_RUNTIME_STATICASSERT_H_INCLUDED
