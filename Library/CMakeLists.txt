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

cmake_minimum_required(VERSION 3.20)

set(name ${Shizu.Runtime.project-name})

begin_library()

list(APPEND ${name}.header_files includes/Shizu/Runtime/Configure.h.in)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Include.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Include.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Gc.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Gc.private.h)
list(APPEND ${name}.source_files sources/Shizu/Runtime/Gc.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Gc.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Value.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Value.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Type.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Type.h)
list(APPEND ${name}.source_files sources/Shizu/Runtime/Type.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Type.private.h)

list(APPEND ${name}.header_files includes/Shizu/Runtime/Types/_SmallTypeArray.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Stack.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Stack.private.h)
list(APPEND ${name}.source_files sources/Shizu/Runtime/Stack.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Stack.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/State.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/State.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/State1.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/State1.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/JumpTarget.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/JumpTarget.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Locks.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Locks.h)
list(APPEND ${name}.source_files sources/Shizu/Runtime/Locks.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Locks.private.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Objects/List.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/List.private.h)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/List.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Objects/Map.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/Map.private.h)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/Map.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Objects/String.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/String.private.h)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/String.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Objects/WeakReference.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/WeakReference.private.h)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/WeakReference.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Objects/ByteArray.private.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/ByteArray.private.h)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Objects/ByteArray.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/Status.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/Status.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/DebugAssert.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/DebugAssert.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/StaticAssert.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/StaticAssert.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/NoReturn.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/NoReturn.h)

list(APPEND ${name}.source_files sources/Shizu/Runtime/getWorkingDirectory.c)
list(APPEND ${name}.header_files includes/Shizu/Runtime/getWorkingDirectory.h)

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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includes/Shizu/Runtime/Configure.h.in includes/Shizu/Runtime/Configure.h)
list(APPEND ${name}.configuration_files ${CMAKE_CURRENT_BINARY_DIR}/includes/Shizu/Runtime/Configure.h)

end_library()

define_property(TARGET PROPERTY modules
  BRIEF_DOCS "My new custom property"
)

set_target_properties(Shizu.Runtime PROPERTIES modules "")

target_link_libraries(${name} idlib-file-system idlib-bit-scan idlib-process)

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

# We must define _GNU_SOURCE under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()
