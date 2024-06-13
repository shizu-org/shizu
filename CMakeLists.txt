#
# Shizu Runtime
# Copyright (C) 2023-2024 Michael Heilmann. All rights reserved.
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

# Set the project name.
project(Shizu.Runtime LANGUAGES C)
set(Shizu.Runtime.project-name "Shizu.Runtime")

# To simply things.
set(name "Shizu.Runtime")

# Copy the files of the rendition to the interpreter binary directory.
# rendition_name The rendition name.
macro(copy_plugin target module)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake -E copy_if_different..."
            $<TARGET_FILE:${module}>                                                  # file to copy
            $<TARGET_FILE_DIR:${target}>                                              # target directory
  )
endmacro()

macro(copy_assets target module)
  file(GLOB_RECURSE assets_to_copy RELATIVE $<SOURCE_DIR:${module}>/Assets $<SOURCE_DIR:${module}>/Assets/*.* )
  message(STATUS " - ${module} assets: ${assets_to_copy}")
endmacro()

include(FetchContent)

#######################################################################################################################

set(idlib-process.repository CACHE STRING "IdLib Process: The Git repository to check out")
if (NOT idlib-process.repository)
  set(idlib-process.repository https://github.com/michaelheilmann/idlib-process.git)
endif()

set(idlib-process.tag CACHE STRING "IdLib Process: The Git tag to check out")
if (NOT idlib-process.tag)
  set(idlib-process.tag 1.6)
endif()

set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.6-src")
endif()

set(idlib-process.binary-dir CACHE STRING "IdLib Process: Path to the build directory")
if (NOT idlib-process.binary-dir)
  set(idlib-process.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.6-bld")
endif()
                    
#######################################################################################################################

set(idlib-bit-scan.repository CACHE STRING "IdLib Bit Scan: The Git repository to check out")
if (NOT idlib-bit-scan.repository)
  set(idlib-bit-scan.repository https://github.com/michaelheilmann/idlib-bit-scan.git)
endif()

set(idlib-bit-scan.tag CACHE STRING "IdLib Bit Scan: The Git tag to check out")
if (NOT idlib-bit-scan.tag)
  set(idlib-bit-scan.tag 1.5)
endif()

set(idlib-bit-scan.source-dir CACHE STRING "IdLib Bit Scan: Path to the source directory")
if (NOT idlib-bit-scan.source-dir)
  set(idlib-bit-scan.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.5-src")
endif()

set(idlib-bit-scan.binary-dir CACHE STRING "IdLib Bit Scan: Path to the build directory")
if (NOT idlib-bit-scan.binary-dir)
  set(idlib-bit-scan.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.5-bld")
endif()

FetchContent_Declare(idlib-bit-scan
                     GIT_REPOSITORY ${idlib-bit-scan.repository}
                     GIT_TAG ${idlib-bit-scan.tag}
                     SOURCE_DIR ${idlib-bit-scan.source-dir}
                     BINARY_DIR ${idlib-bit-scan.binary-dir})

#######################################################################################################################

set(idlib-file-system.repository CACHE STRING "IdLib File System: The Git repository to check out")
if (NOT idlib-file-system.repository)
  set(idlib-file-system.repository https://github.com/michaelheilmann/idlib-file-system.git)
endif()

set(idlib-file-system.tag CACHE STRING "IdLib File System: The Git tag to check out")
if (NOT idlib-file-system.tag)
  set(idlib-file-system.tag 1.11)
endif()

set(idlib-file-system.source-dir CACHE STRING "IdLib File System: Path to the source directory")
if (NOT idlib-file-system.source-dir)
  set(idlib-file-system.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.11-src")
endif()

set(idlib-file-system.binary-dir CACHE STRING "IdLib File System: Path to the build directory")
if (NOT idlib-file-system.binary-dir)
  set(idlib-file-system.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.11-bld")
endif()

FetchContent_Declare(idlib-file-system
                     GIT_REPOSITORY ${idlib-file-system.repository}
                     GIT_TAG ${idlib-file-system.tag}
                     SOURCE_DIR ${idlib-file-system.source-dir}
                     BINARY_DIR ${idlib-file-system.binary-dir})

#######################################################################################################################

set(idlib-byte-sequence.repository CACHE STRING "IdLib Byte Sequence: The Git repository to check out")
if (NOT idlib-byte-sequence.repository)
  set(idlib-byte-sequence.repository https://github.com/michaelheilmann/idlib-byte-sequence.git)
endif()

set(idlib-byte-sequence.tag CACHE STRING "IdLib Byte Sequence: The Git tag to check out")
if (NOT idlib-byte-sequence.tag)
  set(idlib-byte-sequence.tag 1.1)
endif()

set(idlib-byte-sequence.source-dir CACHE STRING "IdLib Byte Sequence: Path to the source directory")
if (NOT idlib-byte-sequence.source-dir)
  set(idlib-byte-sequence.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-byte-sequence-1.1-src")
endif()

set(idlib-byte-sequence.binary-dir CACHE STRING "IdLib Byte Sequence: Path to the build directory")
if (NOT idlib-byte-sequence.binary-dir)
  set(idlib-byte-sequence.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-byte-sequence-1.1-bld")
endif()

FetchContent_Declare(idlib-byte-sequence
                     GIT_REPOSITORY ${idlib-byte-sequence.repository}
                     GIT_TAG ${idlib-byte-sequence.tag}
                     SOURCE_DIR ${idlib-byte-sequence.source-dir}
                     BINARY_DIR ${idlib-byte-sequence.binary-dir})

#######################################################################################################################

FetchContent_MakeAvailable(idlib-bit-scan idlib-byte-sequence idlib-file-system)
                                                                                                                       
#######################################################################################################################

# Include the cmake library.
include(${idlib-process.source-dir}/cmake/all.cmake)
include(CMake/all.cmake)

# Enable testing.
enable_testing()
add_subdirectory(Library)
add_subdirectory(Modules)
add_subdirectory(Test)
