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

#include "Shizu/Runtime/CxxUtilities.h"

typedef enum Shizu_Status Shizu_Status;

enum Shizu_Status {

  /// @brief No error.
  Shizu_Status_NoError = 0,
  /// @brief The type of an argument is invalid.
  Shizu_Status_ArgumentTypeInvalid,
  /// @brief The number of arguments is invalid.
  Shizu_Status_NumberOfArgumentsInvalid,
  /// @brief A numeric argument value is out of range.
  Shizu_Status_ArgumentOutOfRange,
  /// @brief A non-numeric argument value is invalid.
  Shizu_Status_ArgumentValueInvalid,
  /// @brief A method was not found.
  Shizu_Status_MethodNotFound,
  /// @brief An allocation failed.
  Shizu_Status_AllocationFailed,
  /// @brief A runtime test failed.
  Shizu_Status_RuntimeTestFailed,
  /// @brief A lexical error occurred.
  Shizu_Status_LexicalError,
  /// @brief A syntactical error occurred.
  Shizu_Status_SyntacticalError,
  /// @brief A semantical error occurred.
  Shizu_Status_SemanticalError,

  Shizu_Status_OperationInvalid,

  Shizu_Status_UnreachabelCodeReached,

  Shizu_Status_StackOverflow,

  Shizu_Status_StackUnderflow,

  Shizu_Status_StackIndexOutOfBounds,

  /// Status value indicating a division by zero occurred.
  Shizu_Status_DivisionByZero,

  /// Status value indicating an encoding is invalid.
  Shizu_Status_InvalidEncoding,

  Shizu_Status_StackCorruption,



  Shizu_Status_EnvironmentFailed,

  Shizu_Status_ConversionFailed,

  Shizu_Status_NotExists,

  Shizu_Status_Exists,

  // Raised if a dispatch does not exist.
  Shizu_Status_DispatchNotExists,

  // Raised if a method is not implemented.
  // This is different from Shizu_Status_MethodNotDefined.
  Shizu_Status_MethodNotImplemented,

}; // enum Shizu_Status

#endif // SHIZU_RUNTIME_STATUS_H_INCLUDED
