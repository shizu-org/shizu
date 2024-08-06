#if !defined(SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_PARSER_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_PARSER_H_INCLUDED

#include "Shizu/Runtime/Value.h"
#include "Shizu/Runtime/State2.h"
typedef struct Shizu_String Shizu_String;

typedef void Shizu_Operations_StringToInteger_Parser_CallbackContext;

// Invoked when the significand was parsed. This function must be invoked exactly once during a successful parser.
// @param sign true if positive, false if negative.
// @param integralDigits Pointer to the integral digits.
// @param integralDigitsLength The number of integral digits. Can be zero iff fractionalDigitsCount is non-zero.
typedef void (Shizu_Operations_StringToInteger_Parser_CallbackFunction)
  (
    Shizu_State2* state,
    Shizu_Operations_StringToInteger_Parser_CallbackContext* context,
    bool sign,
    char const* integerPart,
    size_t integerPartLength
  );

void
Shizu_Operations_StringToInteger_Parser_parse
  (
    Shizu_State2* state,
    Shizu_String* source,
    Shizu_Operations_StringToInteger_Parser_CallbackContext* callbackContext,
    Shizu_Operations_StringToInteger_Parser_CallbackFunction* callbackFunction
  );

#endif // SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_PARSER_H_INCLUDED
