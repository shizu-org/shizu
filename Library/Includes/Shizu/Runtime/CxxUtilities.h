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

#if !defined(SHIZU_RUNTIME_CXXUTILITIES_H_INCLUDED)
#define SHIZU_RUNTIME_CXXUTILITIES_H_INCLUDED

#include "Shizu/Runtime/Configure.h"

// bool, true, false
#include <stdbool.h>

// size_t
#include <stddef.h>

typedef struct Shizu_SourceLocationC {
  char const* file;
  int line;
} Shizu_SourceLocationC;

#define Shizu_SourceFileC() \
  __FILE__

#define Shizu_SourceLineC() \
  __LINE__

#define Shizu_SourceLocationC() \
  (Shizu_SourceLocationC) {. file = Shizu_SourceFileC(), .line = Shizu_SourceLineC() }

/// @since 1.0
/// Function annotation indicating a function does not return normally.
/// The function either terminates the program (cf. exit) or returns via a jump (cf. longjmp).
#if Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
#define Shizu_NoReturn() __declspec(noreturn)
#else
#define Shizu_NoReturn()
#endif

#if defined(_DEBUG)

  /// @since 1.0
  /// @internal
  Shizu_NoReturn() void
  Shizu_debugAssertionFailed
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

  #define Shizu_debugAssert(expression) \
    if (!(expression)) {\
      Shizu_debugAssertionFailed(__FILE__, __LINE__, #expression); \
    }

#else

  #define Shizu_debugAssert(expression) \
    ;

#endif

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


/// @since 1.90
/// @brief Print an error message and terminate the program.
/// If this is invoked, then the user discovered a bug.
Shizu_NoReturn() void Shizu_unreachableCodeReached(char const* file, int line);

#endif // SHIZU_RUNTIME_CXXUTILITIES_H_INCLUDED
