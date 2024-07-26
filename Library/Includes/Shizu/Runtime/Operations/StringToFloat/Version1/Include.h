#if !defined(SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_INCLUDE_H_INCLUDED)
#define SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_INCLUDE_H_INCLUDED

#include "Shizu/Runtime/Value.h"
typedef struct Shizu_String Shizu_String;

Shizu_Float32
Shizu_Operations_StringToFloat32_Version1_convert
  (
    Shizu_State2* state,
    Shizu_String* source
  );
  
#if defined(Shizu_Configuration_WithTests)

void
Shizu_Operations_StringToFloat_Version1_tests
  (
    Shizu_State2* state
  );

#endif

#endif // SHIZU_RUNTIME_OPERATIONS_STRINGTOFLOAT_VERSION1_INCLUDE_H_INCLUDED
