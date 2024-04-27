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

#if !defined(SHIZU_RUNTIME_STATUS_H_INCLUDED)
#define SHIZU_RUNTIME_STATUS_H_INCLUDED

typedef enum Shizu_Status Shizu_Status;

enum Shizu_Status {

  Shizu_Status_NoError = 0,

  Shizu_Status_ArgumentInvalid,
  
  Shizu_Status_OperationInvalid,
  
  Shizu_Status_AllocationFailed,

  Shizu_Status_UnreachabelCodeReached,

  Shizu_Status_StackOverflow,

  Shizu_Status_StackUnderflow,

  Shizu_Status_StackIndexOutOfBounds,
  
  Shizu_Status_StackCorruption,
    
  Shizu_Status_LexicalError,

  Shizu_Status_SyntacticalError,

  Shizu_Status_SemanticalError,

  Shizu_Status_EnvironmentFailed,

  Shizu_Status_ConversionFailed,

}; // enum Shizu_Status

#endif // SHIZU_RUNTIME_STATUS_H_INCLUDED
