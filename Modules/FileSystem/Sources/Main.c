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

Shizu_Module_Export void
Shizu_Module_load
  (
    Shizu_State* state
  )
{ fprintf(stdout, "[Module : File System] loaded\n"); }

Shizu_Module_Export void
Shizu_Module_unload
  (
    Shizu_State* state
  )
{ fprintf(stdout, "[Module : File System] unloaded\n"); }

Shizu_Module_Export char const*
Shizu_Module_getName
  (
    Shizu_State* state
  )
{
  static const char* NAME = "Shizu Module File System";
  return NAME;
}
