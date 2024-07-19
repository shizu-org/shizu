#include "Shizu/Runtime/Extensions.h"

#include "Shizu/Runtime/Include.h"

Shizu_List*
Shizu_Runtime_Extensions_createList
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void), };
  Shizu_Value_setType(&argumentValues[0], Shizu_List_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_List*)Shizu_Value_getObject(&returnValue);
}

Shizu_Map*
Shizu_Runtime_Extensions_createMap
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void),};
  Shizu_Value_setType(&argumentValues[0], Shizu_Map_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_Map*)Shizu_Value_getObject(&returnValue);
}

Shizu_Environment*
Shizu_Runtime_Extensions_createEnvironment
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void), };
  Shizu_Value_setType(&argumentValues[0], Shizu_Environment_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_Environment*)Shizu_Value_getObject(&returnValue);
}

Shizu_Environment*
Shizu_Runtime_Extensions_getOrCreateEnvironment
  (
    Shizu_State2* state,
    Shizu_Environment* environment,
    Shizu_String* name
  )
{
  if (!Shizu_Environment_isDefined(state, environment, name)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_Runtime_Extensions_createEnvironment(state));
    Shizu_Environment_set(state, environment, name, &temporary);
  }
  environment = Shizu_Environment_getEnvironment(state, environment, name);
  return environment;
}

Shizu_String*
Shizu_Runtime_Extensions_getStringValue
  (
    Shizu_State2* state,
    Shizu_Value* value
  )
{
  if (!Shizu_Value_isObject(value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Object* objectValue = Shizu_Value_getObject(value);
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state),
                               Shizu_Object_getObjectType(state, objectValue), Shizu_String_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return (Shizu_String*)objectValue ;
}

Shizu_Float32
Shizu_Runtime_Extensions_getFloat32Value
  (
    Shizu_State2* state,
    Shizu_Value* value
  )
{
  if (!Shizu_Value_isFloat32(value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getFloat32(value);
}

Shizu_Integer32
Shizu_Runtime_Extensions_getInteger32Value
  (
    Shizu_State2* state,
    Shizu_Value* value
  )
{
  if (!Shizu_Value_isInteger32(value)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getInteger32(value);
}

Shizu_WeakReference*
Shizu_Runtime_Extensions_createWeakReference
  (
    Shizu_State2* state,
    Shizu_Object* reference
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void), Shizu_Value_InitializerVoid(Shizu_Void_Void), };
  Shizu_Value_setType(&argumentValues[0], Shizu_WeakReference_getType(state));
  if (reference) {
    Shizu_Value_setObject(&argumentValues[1], reference);
  } else {
    Shizu_Value_setVoid(&argumentValues[1], Shizu_Void_Void);
  }
  Shizu_Operations_create(state, &returnValue, 2, &argumentValues[0]);
  return (Shizu_WeakReference*)Shizu_Value_getObject(&returnValue);
}

Shizu_Boolean
Shizu_Runtime_Extensions_isFloat32
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{ return Shizu_Value_isFloat32(&value); }

Shizu_Boolean
Shizu_Runtime_Extensions_isBoolean
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{
  return Shizu_Value_isBoolean(&value);
}

Shizu_Boolean
Shizu_Runtime_Extensions_isInteger32
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{ return Shizu_Value_isInteger32(&value); }

Shizu_Boolean
Shizu_Runtime_Extensions_isList
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{
  if (!Shizu_Value_isObject(&value)) {
    return Shizu_Boolean_False;
  }
  Shizu_Object* o = Shizu_Value_getObject(&value);
  return Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), o->type, Shizu_List_getType(state));
}

Shizu_Boolean
Shizu_Runtime_Extensions_isMap
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{
  if (!Shizu_Value_isObject(&value)) {
    return Shizu_Boolean_False;
  }
  Shizu_Object* o = Shizu_Value_getObject(&value);
  return Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), o->type, Shizu_Map_getType(state));
}

Shizu_Boolean
Shizu_Runtime_Extensions_isString
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{
  if (!Shizu_Value_isObject(&value)) {
    return Shizu_Boolean_False;
  }
  Shizu_Object* o = Shizu_Value_getObject(&value);
  return Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), o->type, Shizu_String_getType(state));
}

Shizu_Boolean
Shizu_Runtime_Extensions_isVoid
  (
    Shizu_State2* state,
    Shizu_Value value
  )
{ return Shizu_Value_isVoid(&value); }
