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

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name Shizu.Runtime.Modules.FileSystem)
Shizu_beginDynamicLibrary()

list(APPEND ${name}.source_files Sources/Main.c)

list(APPEND ${name}.source_files Sources/FileSystem.c)
list(APPEND ${name}.header_files Includes/FileSystem.h)

list(APPEND ${name}.source_files Sources/FileSystem/getWorkingDirectory.c)
list(APPEND ${name}.header_files Includes/FileSystem/getWorkingDirectory.h)

list(APPEND ${name}.source_files Sources/FileSystem/setFileContents.c)
list(APPEND ${name}.header_files Includes/FileSystem/setFileContents.h)

list(APPEND ${name}.source_files Sources/FileSystem/getFileContents.c)
list(APPEND ${name}.header_files Includes/FileSystem/getFileContents.h)

list(APPEND ${name}.source_files Sources/FileSystem/Utilities.c)
list(APPEND ${name}.header_files Includes/FileSystem/Utilities.h)

Shizu_endDynamicLibrary()

target_link_libraries(${name} Shizu.Runtime)
add_dependencies(${name} Shizu.Runtime)

# For each MyTarget in this list and your executable target YourTarget you need to copy <$TARGET_FILE:MyTarget> to <TARGET_FILE_DIR:YourTarget>.
set(modules "")
get_target_property(modules Shizu.Runtime modules)
list(APPEND modules ${name})
set_target_properties(Shizu.Runtime PROPERTIES modules ${modules})
