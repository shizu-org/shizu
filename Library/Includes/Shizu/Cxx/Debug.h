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

#if !defined(SHIZU_CXX_DEBUG_H_INCLUDED)
#define SHIZU_CXX_DEBUG_H_INCLUDED

#if !defined(SHIZU_CXX_PRIVATE) && 1 != SHIZU_CXX_PRIVATE
  #error("Do not include `Shizu/Cxx/Debug.h` directly. Include `Shizu/Cxx/Include.h` instead.")
#endif

#include "Shizu/Cxx/NoReturn.h"

#if defined(_DEBUG)

#include <stdarg.h>

/// @since 1.0
/// @internal
/// @brief Write to the log.
/// @param state A pointer to the Shizu_State1.
/// @param format, ... printf-style arguments.
void
Shizu_Cxx_Debug_printf
  (
    char const* format,
    ...
  );

/// @since 1.0
/// @internal
/// @brief Write to the log.
/// @param state A pointer to the Shizu_State1.
/// @param format, arguments printf-style arguments.
void
Shizu_Cxx_Debug_printfv
  (
    char const* format,
    va_list arguments
  );

#endif

#if defined(_DEBUG)

/// @since 1.0
/// @internal
/// @brief Invoked by assertion failed.
/// @param file, line The C source file and the line in that file.
/// @param expression The expression.
Shizu_Cxx_NoReturn() void
Shizu_Cxx_Debug_assertionFailed
  (
    char const* file,
    int line,
    char const* expression
  );

#endif

/// @since 1.0
/// @brief Macro terminating the program with failure if an expression evaluates to false.
/// @param expression An expression that can be converted into logically true or logically false. 
/// @remarks This macro evaluates to the empty statement if _DEBUG is not defined.
#if defined(_DEBUG)

  #define Shizu_Cxx_Debug_assert(expression) \
      if (!(expression)) {\
        Shizu_Cxx_Debug_assertionFailed(__FILE__, __LINE__, #expression); \
      }

#else

  #define Shizu_Cxx_Debug_assert(expression) \
            ;

#endif

#endif // SHIZU_CXX_DEBUG_H_INCLUDED
