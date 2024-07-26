#
# Shizu
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

cmake_minimum_required(VERSION 3.29)

# Set the project name.
project(Shizu LANGUAGES C)
set(Shizu.project-name "Shizu")

# To simply things.
set(name "Shizu")

# Enable testing.
enable_testing()

#######################################################################################################################
set(idlib-process.with-tests CACHE BOOL "Idlib Process: Enable Tests")
if (NOT idlib-process.with-tests)
  set(idlib-process.with-tests ON)
endif()
set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/IdLib.Process")
endif()
add_subdirectory(${idlib-process.source-dir})
#######################################################################################################################
set(idlib-byte-sequence.with-tests CACHE BOOL "Idlib Byte Sequence: Enable Tests")
if (NOT idlib-byte-sequence.with-tests)
  set(idlib-byte-sequence.with-tests ON)
endif()
set(idlib-byte-sequence.source-dir CACHE STRING "IdLib Byte Sequence: Path to the source directory")
if (NOT idlib-byte-sequence.source-dir)
  set(idlib-byte-sequence.source-dir "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/IdLib.ByteSequence")
endif()
add_subdirectory(${idlib-byte-sequence.source-dir})
#######################################################################################################################
set(idlib-file-system.with-tests CACHE BOOL "Idlib File System: Enable Tests")
if (NOT idlib-file-system.with-tests)
  set(idlib-file-system.with-tests ON)
endif()
set(idlib-file-system.source-dir CACHE STRING "IdLib File System: Path to the source directory")
if (NOT idlib-file-system.source-dir)
  set(idlib-file-system.source-dir "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/IdLib.FileSystem")
endif()
add_subdirectory(${idlib-file-system.source-dir})
#######################################################################################################################

# Include the cmake library.
include(CMake/all.cmake)

add_subdirectory(Specification)
add_subdirectory(Library)
add_subdirectory(Modules)
add_subdirectory(Test)
