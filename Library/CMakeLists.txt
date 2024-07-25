#
# Shizu
# Copyright (C) 2024 Michael Heilmann. All rights reserved.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

set(name ${Shizu.project-name})

Shizu_beginLibrary()

list(APPEND ${name}.header_files Includes/Shizu/Runtime/Configure.h.in)

list(APPEND ${name}.source_files Sources/Shizu/Gc/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Gc/Include.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Include.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Extensions.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Extensions.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Gc.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Gc.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Gc.private.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Value.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Value.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Type.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Type.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Type.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Type.private.h)

list(APPEND ${name}.header_files Includes/Shizu/Runtime/Types/_SmallTypeArray.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Stack.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Stack.private.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Stack.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Stack.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/State1.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/State1.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/State2.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/State2.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/JumpTarget.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/JumpTarget.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Locks.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Locks.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Locks.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Locks.private.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/List.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/List.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/List.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/Map.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Map.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Map.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/String.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/String.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/String.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/WeakReference.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/WeakReference.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/WeakReference.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/ByteArray.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/ByteArray.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/ByteArray.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Status.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Status.h)

list(APPEND ${name}.source_files Sources/Shizu/Cxx/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/Include.h)
list(APPEND ${name}.source_files Sources/Shizu/Cxx/StaticAssert.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/StaticAssert.h)
list(APPEND ${name}.source_files Sources/Shizu/Cxx/Debug.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/Debug.h)
list(APPEND ${name}.source_files Sources/Shizu/Cxx/NoReturn.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/NoReturn.h)
list(APPEND ${name}.source_files Sources/Shizu/Cxx/UnreachableCodeReached.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/UnreachableCodeReached.h)
list(APPEND ${name}.source_files Sources/Shizu/Cxx/SourceLocation.c)
list(APPEND ${name}.header_files Includes/Shizu/Cxx/SourceLocation.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/getWorkingDirectory.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/getWorkingDirectory.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/Environment.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Environment.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Environment.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/CxxProcedure.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/CxxProcedure.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/BigInteger.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigInteger.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/BigInteger/fromString.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigInteger/fromString.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/BigInteger/fromInteger32.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigInteger/fromInteger32.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/BigInteger/fromInteger64.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigInteger/fromInteger64.h)
list(APPEND ${name}.header_files Sources/Shizu/Runtime/Objects/BigInteger/compareMagnitudes.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigInteger/compareMagnitudes.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/BigFloat.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/BigFloat.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Module.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Module.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Object.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Object.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Operations/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Operations/Include.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Operations/ToInteger.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Operations/ToInteger.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Operations/ToFloat32.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Operations/ToFloat32.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Types/_SmallTypeArray.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Types/_SmallTypeArray.h)

# Configuration variables for 'Configure.h.in'.
if (NOT DEFINED ${name}.operating_system)
  message(FATAL_ERROR "`${name}.operating_system` not defined")
endif()
if (${${name}.operating_system} STREQUAL ${${name}.operating_system_windows})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Windows)
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Linux)
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_cygwin})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Cygwin)
else()
  message(FATAL_ERROR "invalid value for `${name}.operating_system`")
endif()

if (NOT DEFINED ${name}.compiler_c)
  message(FATAL_ERROR "`${name}.compiler_c` not defined")
endif()
if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Msvc)
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_gcc})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Gcc)
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_clang})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Clang)
else()
  message(FATAL_ERROR "invalid value for `${name}.compiler_c`")
endif()

if (NOT DEFINED ${name}.instruction_set_architecture)
  message(FATAL_ERROR "`${name}.instruction_set_architecture` not defined")
endif()
if (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x64})
  set(Shizu_Configuration_InstructionSetArchitecture Shizu_Configuration_InstructionSetArchitecture_X64)
elseif (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x86})
  set(Shizu_Configuration_InstructionSetArchitecture Shizu_Configuration_InstructionSetArchitecture_X86)
else()
  message(FATAL_ERROR "invalid value for `${name}.instrunction_set_architecture`")
endif()

option(${name}.with_integer64 "enable Integer64 type" ON)
if (NOT DEFINED ${name}.with_integer64)
  message(FATAL_ERROR "`${name}.with_integer64` not defined")
endif()
if (${${name}.with_integer64} STREQUAL "ON")
  set(Shizu_Configuration_WithInteger64 1)
else()
  set(Shizu_Configuration_WithInteger64 0)
#else()
#message(FATAL_ERROR "invalid value `${${name}.with_integer64}` for `${name}.with_integer64`")
endif()

option(${name}.with_float64 "enable Float64 type" ON)
if (NOT DEFINED ${name}.with_float64)
  message(FATAL_ERROR "`${name}.with_float64` not defined")
endif()
if (${${name}.with_float64})
  set(Shizu_Configuration_WithFloat64 1)
else()
  set(Shizu_Configuration_WithFloat64 0)
#else()
#message(FATAL_ERROR "invalid value `${${name}.with_float64}` for `${name}.with_float64`")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Includes/Shizu/Runtime/Configure.h.in Includes/Shizu/Runtime/Configure.h)
list(APPEND ${name}.configuration_files ${CMAKE_CURRENT_BINARY_DIR}/Includes/Shizu/Runtime/Configure.h)

Shizu_endLibrary()

set_target_properties(Shizu PROPERTIES modules "")

target_link_libraries(${name} IdLib.FileSystem IdLib.BitScan IdLib.ByteSequence idlib-process)

# We must define _GNU_SOURCE under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()
