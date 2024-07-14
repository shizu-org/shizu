#if !defined(SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED)
#define SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED

#include "Shizu/Runtime/Value.h"
typedef struct Shizu_Environment Shizu_Environment;
typedef struct Shizu_List Shizu_List;
typedef struct Shizu_Map Shizu_Map;
typedef struct Shizu_String Shizu_String;
typedef struct Shizu_State2 Shizu_State2;

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export
#else
  #error("operating system not (yet) supported")
#endif

// Utility function to create a "Shizu.List".
Shizu_List*
Shizu_Runtime_Extensions_createList
  (
    Shizu_State2* state
  );

// Utility function to create a "Shizu.Map".
Shizu_Map*
Shizu_Runtime_Extensions_createMap
  (
    Shizu_State2* state
  );

// Utility function to create a "Shizu.Environment".
Shizu_Environment*
Shizu_Runtime_Extensions_createEnvironment
  (
    Shizu_State2* state
  );

// Utility function to get or create a "Shizu.Environment".
Shizu_Environment*
Shizu_Runtime_Extensions_getOrCreateEnvironment
  (
    Shizu_State2* state,
    Shizu_Environment* environment,
    Shizu_String* name
  );

// Load a "String" value from a value. Raise "InvalidArgumentType" if the value does not hold a "String" value.
Shizu_String*
Shizu_Runtime_Extensions_getStringValue
  (
    Shizu_State2* state,
    Shizu_Value* value  
  );

#endif // SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED
