#if !defined(SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_PARSER_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_PARSER_H_INCLUDED

#include "Shizu/Runtime/Value.h"
#include "Shizu/Runtime/State2.h"
typedef struct Shizu_String Shizu_String;

typedef void Shizu_Operations_StringToFloat_Parser_CallbackContext;

// Invoked when a floating pointer number literal was parsed.
// This function must be invoked exactly once during a successful parser.
// @param sign true if positive, false if negative.
// @param significandDigits Pointer to the significand digits.
// @param significandDigitsLength The number of significand digits. Can be zero iff significandDigitsCount is non-zero.
// @param fractionalDigits Pointer to the fractional digits.
// @param fractionalDigitsLength The number of fractional digits. Can be zero iff integralDigitsCount is non-zero.
// @param exponentSign true if positive, false if negative.
// @param exponentDigits Pointer to the exponent digits.
// @param exponentDigitsLength The number of exponentDigits. Can be zero.
typedef void (Shizu_Operations_StringToFloat_Parser_CallbackFunction)
  (
    Shizu_State2* state,
    Shizu_Operations_StringToFloat_Parser_CallbackContext* context,
    bool sign,
    char const* significandPart,
    size_t significandPartLength,
    char const* fractionalPart,
    size_t fractionalPartLength,
    bool exponentSign,
    char const* exponentDigits,
    size_t exponentDigitsLength
  );

void
Shizu_Operations_StringToFloat_Parser_parse
  (
    Shizu_State2* state,
    Shizu_String* source,
    Shizu_Operations_StringToFloat_Parser_CallbackContext* callbackContext,
    Shizu_Operations_StringToFloat_Parser_CallbackFunction* callbackFunction
  );

#endif // SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_PARSER_H_INCLUDED
