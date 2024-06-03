#include "FileSystem.h"

// fprintf, stdio
#include <stdio.h>

// strlen
#include <string.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Module_Export
#else
  #error("operating system not (yet) supported")
#endif

#include "FileSystem/deleteFile.h"
#include "FileSystem/getFileContents.h"
#include "FileSystem/getWorkingDirectory.h"
#include "FileSystem/setFileContents.h"

Shizu_Module_Export void
Shizu_ModuleLibrary_load
  (
    Shizu_State* state
  )
{
  Shizu_Dl* dl = NULL;
  Shizu_JumpTarget jumpTarget;

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State_getDlByAdr(state, &deleteFile);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &deleteFile, dl));
    Shizu_Environment_set(state, Shizu_State_getGlobals(state), Shizu_String_create(state, "deleteFile", strlen("deleteFile")), &value);
    Shizu_State_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
  } else {
    Shizu_State_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
    }
    Shizu_State_jump(state);
  }

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State_getDlByAdr(state, &getFileContents);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &getFileContents, dl));
    Shizu_Environment_set(state, Shizu_State_getGlobals(state), Shizu_String_create(state, "getFileContents", strlen("getFileContents")), &value);
    Shizu_State_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
  } else {
    Shizu_State_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
    }
    Shizu_State_jump(state);
  }

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State_getDlByAdr(state, &getWorkingDirectory);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &getWorkingDirectory, dl));
    Shizu_Environment_set(state, Shizu_State_getGlobals(state), Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")), &value);
    Shizu_State_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
  } else {
    Shizu_State_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
    }
    Shizu_State_jump(state);
  }

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    dl = Shizu_State_getDlByAdr(state, &setFileContents);
    Shizu_Value value;
    Shizu_Value_setObject(&value, (Shizu_Object*)Shizu_CxxProcedure_create(state, &setFileContents, dl));
    Shizu_Environment_set(state, Shizu_State_getGlobals(state), Shizu_String_create(state, "setFileContents", strlen("setFileContents")), &value);
    Shizu_State_popJumpTarget(state);
    Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
  } else {
    Shizu_State_popJumpTarget(state);
    if (dl) {
      Shizu_State1_unrefDl(Shizu_State_getState1(state), dl);
    }
    Shizu_State_jump(state);
  }

  fprintf(stdout, "[Module : File System] loaded\n");
}

Shizu_Module_Export void
Shizu_ModuleLibrary_unload
  (
    Shizu_State* state
  )
{
  fprintf(stdout, "[Module : File System] unloaded\n");
}

Shizu_Module_Export char const*
Shizu_ModuleLibrary_getName
  (
    Shizu_State1* state
  )
{
  static const char* NAME = "Shizu Module File System";
  return NAME;
}

Shizu_Module_Export
Shizu_ModuleLibrary_update
  (
    Shizu_State* state
  )
{ fprintf(stdout, "[Module : FileSystem] update\n"); }
