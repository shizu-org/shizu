#if !defined(SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED)
#define SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED

#include "Shizu/Runtime/Value.h"
typedef struct Shizu_Environment Shizu_Environment;
typedef struct Shizu_List Shizu_List;
typedef struct Shizu_Map Shizu_Map;
typedef struct Shizu_String Shizu_String;
typedef struct Shizu_State2 Shizu_State2;
typedef struct Shizu_WeakReference Shizu_WeakReference;

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

// Load a "Float32" value from a value.
// Raise "InvalidArgumentType" if the value does not hold a "Float32" value.
Shizu_Float32
Shizu_Runtime_Extensions_getFloat32Value
  (
    Shizu_State2* state,
    Shizu_Value* value
  );

// Load an "Integer32" value from a value.
// Raise "InvalidArgumentType" if the value does not hold a "Integer32" value.
Shizu_Integer32
Shizu_Runtime_Extensions_getInteger32Value
  (
    Shizu_State2* state,
    Shizu_Value* value
  );

// Utility function to get or create a "Shizu.WeakReference". The constructor "Shizu.WeakReference.construct(reference :
// Object|Void)" is invoked with the "Object" value to which "reference" points to or "void" if "reference" is a null
// pointer.
Shizu_WeakReference*
Shizu_Runtime_Extensions_createWeakReference
  (
    Shizu_State2* state,
    Shizu_Object* reference
  );


Shizu_Boolean
Shizu_Runtime_Extensions_isBoolean
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isFloat32
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isInteger32
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isList
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isMap
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isString
  (
    Shizu_State2* state,
    Shizu_Value value
  );

Shizu_Boolean
Shizu_Runtime_Extensions_isVoid
  (
    Shizu_State2* state,
    Shizu_Value value
  );

#endif // SHIZU_RUNTIME_EXTENSIONS_H_INCLUDED
