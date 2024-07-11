#
# Shizu Runtime
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

set(name ${Shizu.Runtime.project-name})

Shizu_beginLibrary()

list(APPEND ${name}.header_files Includes/Shizu/Runtime/Configure.h.in)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Include.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Extensions.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Extensions.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Gc.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Gc.private.h)
list(APPEND ${name}.source_files Sources/Shizu/Runtime/Gc.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Gc.h)

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

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/List.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/List.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/List.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/Map.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Map.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Map.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/String.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/String.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/String.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/WeakReference.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/WeakReference.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/WeakReference.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/ByteArray.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/ByteArray.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/ByteArray.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Status.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Status.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/CxxUtilities.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/CxxUtilities.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/getWorkingDirectory.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/getWorkingDirectory.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/Environment.private.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Environment.private.h)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/Environment.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Objects/CxxProcedure.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Objects/CxxProcedure.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Module.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Module.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Object.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Object.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Operations/Include.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Operations/Include.h)

list(APPEND ${name}.source_files Sources/Shizu/Runtime/Types/_SmallTypeArray.c)
list(APPEND ${name}.header_files Includes/Shizu/Runtime/Types/_SmallTypeArray.h)

# Configuration variables for 'Configure.h.in'.
if (${${name}.operating_system} STREQUAL ${${name}.operating_system_windows})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Windows)
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Linux)
elseif (${${name}.operating_system} STREQUAL ${${name}.operating_system_cygwin})
  set(Shizu_Configuration_OperatingSystem Shizu_Configuration_OperatingSystem_Cygwin)
else()
  message(FATAL_ERROR "operating system detection not executed")
endif()

if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Msvc)
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_gcc})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Gcc)
elseif (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_clang})
  set(Shizu_Configuration_CompilerC Shizu_Configuration_CompilerC_Clang)
else()
  message(FATAL_ERROR "C compiler detection not executed")
endif()

if (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x64})
  set(Shizu_Configuration_InstructionSetArchitecture Shizu_Configuration_InstructionSetArchitecture_X64)
elseif (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x86})
  set(Shizu_Configuration_InstructionSetArchitecture Shizu_Configuration_InstructionSetArchitecture_X86)
else()
  message(FATAL_ERROR "instruction set architecture detection not executed")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Includes/Shizu/Runtime/Configure.h.in Includes/Shizu/Runtime/Configure.h)
list(APPEND ${name}.configuration_files ${CMAKE_CURRENT_BINARY_DIR}/Includes/Shizu/Runtime/Configure.h)

Shizu_endLibrary()

set_target_properties(Shizu.Runtime PROPERTIES modules "")

target_link_libraries(${name} idlib-file-system idlib-bit-scan idlib-byte-sequence idlib-process)

# We must define _GNU_SOURCE under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()
