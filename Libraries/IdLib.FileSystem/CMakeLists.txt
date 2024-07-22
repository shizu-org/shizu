#
# IdLib FileSystem
# Copyright (C) 2018-2024 Michael Heilmann. All rights reserved.
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

project(IdLib.FileSystem LANGUAGES C)

if (NOT idlib-process.source-dir)
  message(FATAL "source directory of IdLib Process is not set")
endif()

add_subdirectory(library)

if (${idlib-file-system.with-tests})
  enable_testing()
  add_subdirectory(test/mock)
  add_subdirectory(test/get_file_type)
  add_subdirectory(test/get_file_contents)
  add_subdirectory(test/set_file_contents)
endif()
