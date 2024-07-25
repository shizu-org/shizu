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

#include "testFromInteger.h"

void
Shizu_Test_BigFloat_testFromInteger32
  (
    Shizu_State2* state
  )
{
  Shizu_BigFloat* x = NULL;

  x = Shizu_BigFloat_createFromInteger32(state, INT32_C(0));

  x = Shizu_BigFloat_createFromInteger32(state, Shizu_Integer32_Minimum);

  x = Shizu_BigFloat_createFromInteger32(state, Shizu_Integer32_Maximum);
}

#if 1 == Shizu_Configuration_WithInteger64

void
Shizu_Test_BigFloat_testFromInteger64
  (
    Shizu_State2* state
  )
{
  Shizu_BigFloat* x = NULL;

  x = Shizu_BigFloat_createFromInteger64(state, INT64_C(0));

  x = Shizu_BigFloat_createFromInteger64(state, Shizu_Integer64_Minimum);

  x = Shizu_BigFloat_createFromInteger64(state, Shizu_Integer64_Maximum);
}

#endif // Shizu_Configuration_WithInteger64
