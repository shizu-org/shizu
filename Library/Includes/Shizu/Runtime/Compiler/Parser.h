#if !defined(SHIZU_RUNTIME_COMPILER_PARSER_H_INCLUDED)
#define SHIZU_RUNTIME_COMPILER_PARSER_H_INCLUDED

#include "Shizu/Runtime/Compiler/Object.h"

Shizu_declareType(Shizu_Runtime_Compiler_Parser)

struct Shizu_Runtime_Compiler_Parser_Dispatch {
  Shizu_Runtime_Compiler_Object_Dispatch _parent;
};

struct Shizu_Runtime_Compiler_Parser {
  Shizu_Runtime_Compiler_Object _parent;
};

#endif // SHIZU_RUNTIME_COMPILER_PARSER_H_INCLUDED
