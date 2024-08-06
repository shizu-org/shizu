#if !defined(SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_INCLUDE_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_INCLUDE_H_INCLUDED

#include "Shizu/Runtime/Value.h"
typedef struct Shizu_String Shizu_String;

Shizu_Integer32
Shizu_Operations_StringToInteger32_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  );

#if 1 == Shizu_Configuration_WithInteger64

Shizu_Integer64
Shizu_Operations_StringToInteger64_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  );

#endif

#endif // SHIZU_RUNTIME_OPERATIONS_STRINGTOINTEGER_VERSION1_INCLUDE_H_INCLUDED
