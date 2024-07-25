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

#if !defined(SHIZU_CXX_SOURCELOCATION_H_INCLUDED)
#define SHIZU_CXX_SOURCELOCATION_H_INCLUDED

#if !defined(SHIZU_CXX_PRIVATE) && 1 != SHIZU_CXX_PRIVATE
  #error("Do not include `Shizu/Cxx/SourceLocation.h` directly. Include `Shizu/Cxx/Include.h` instead.")
#endif

#include "Shizu/Runtime/Configure.h"

// bool, true, false
#include <stdbool.h>

// size_t
#include <stddef.h>

typedef struct Shizu_Cxx_SourceLocation {
  char const* file;
  int line;
} Shizu_Cxx_SourceLocation;

#define Shizu_Cxx_SourceFileName() \
  __FILE__

#define Shizu_Cxx_SourceFileLine() \
  __LINE__

#define Shizu_Cxx_SourceLocation() \
  (Shizu_Cxx_SourceLocation) {. file = Shizu_Cxx_SourceFileName(), .line = Shizu_Cxx_SourceFileLine() }


#endif // SHIZU_CXX_SOURCELOCATION_H_INCLUDED
