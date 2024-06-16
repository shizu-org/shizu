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

set(name Shizu.Runtime.Modules.FileSystem.Tests)
Shizu_beginExecutable()

list(APPEND ${name}.source_files Sources/Main.c)

Shizu_endExecutable()

target_link_libraries(${name} Shizu.Runtime)
add_dependencies(${name} Shizu.Runtime)

add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>)

get_target_property(prerequisiteModules Shizu.Runtime.Modules.FileSystem prerequisiteModules)
list(APPEND prerequisiteModules "Shizu.Runtime.Modules.FileSystem")

message(STATUS " - adding prerequisite modules")
foreach (prerequisiteModule ${prerequisiteModules})
  message(STATUS " - - ${name} <- ${prerequisiteModule}")
  add_dependencies(${name} ${prerequisiteModule})
  copy_plugin(${name} ${prerequisiteModule})
endforeach()

get_target_property(source_dir ${name} SOURCE_DIR)
message(STATUS " - ${name} source dir: ${source_dir}")
file(GLOB_RECURSE assets_to_copy RELATIVE ${source_dir}/Assets ${source_dir}/Assets/*.* )
message(STATUS " - ${name} assets: ${assets_to_copy}")
foreach (asset_to_copy ${assets_to_copy})
    add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${name}>/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            ${source_dir}/Assets/${asset_to_copy}                                     # file to copy
            $<TARGET_FILE_DIR:${name}>/Assets/${asset_to_copy}
    )
endforeach()
