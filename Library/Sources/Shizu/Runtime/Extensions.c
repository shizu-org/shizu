#include "Shizu/Runtime/Extensions.h"

#include "Shizu/Runtime/Include.h"

Shizu_List*
Shizu_Runtime_Extensions_createList
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
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
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Shizu_List_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Shizu_List*)Shizu_Value_getObject(&returnValue);
}

Shizu_Environment*
Shizu_Runtime_Extensions_createEnvironment
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
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
