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

#if !defined(SHIZU_CXX_UNREACHABLECODEREACHED_H_INCLUDED)
#define SHIZU_CXX_UNREACHABLECODEREACHED_H_INCLUDED

#if !defined(SHIZU_CXX_PRIVATE) && 1 != SHIZU_CXX_PRIVATE
  #error("Do not include `Shizu/Cxx/UnreachableCodeReached.h` directly. Include `Shizu/Cxx/Include.h` instead.")
#endif

#include "Shizu/Cxx/NoReturn.h"

/// @since 1.90
/// @brief Print an error message and terminate the program.
/// If this is invoked, then the user discovered a bug.
Shizu_Cxx_NoReturn() void
Shizu_Cxx_onUnreachableCodeReached
  (
    char const* file,
    int line
  );

#define Shizu_Cxx_unreachableCodeReached() \
  Shizu_Cxx_onUnreachableCodeReached(__FILE__, __LINE__)

#endif // SHIZU_CXX_UNREACHABLECODEREACHED_H_INCLUDED
