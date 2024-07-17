#
# Shizu Runtime
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

include(${CMAKE_CURRENT_LIST_DIR}/configure_warnings_and_errors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/define_languages.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_instruction_set_architecture.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_operating_system.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_void_pointer_size.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_compiler.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_multi_target_generator.cmake)

# Begin a (static) library.
macro(Shizu_beginLibrary)

  set(${name}.source_files "")
  set(${name}.header_files "")
  set(${name}.configuration_files "")
  # Asset files are transitive.
  # plugin libraries and executables copy asset files to their output directory.
  set(${name}.asset_files "")

  Shizu_defineLanguages(${name})
  Shizu_detectCompiler_C(${name})
  Shizu_detectCompiler_Cpp(${name})
  Shizu_detectInstructionSetArchitecture(${name})
  Shizu_detectOperatingSystem(${name})
  Shizu_detectMultiTargetGenerator(${name})

endmacro()

# End a (static) library.
macro(Shizu_endLibrary)

  add_library(${name} STATIC)

  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Includes")
  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Includes")

  target_sources(${name} PRIVATE ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})

  Shizu_configureWarningsAndErrors(${name})
  source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})
  
  if (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
    set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
  endif()

endmacro()

# Begin an executable.
macro(Shizu_beginExecutable)

  set(${name}.source_files "")
  set(${name}.header_files "")
  set(${name}.configuration_files "")
  # Asset files are transitive.
  # plugin libraries and executables copy asset files to their output directory.
  set(${name}.asset_files "")

  Shizu_defineLanguages(${name})
  Shizu_detectCompiler_C(${name})
  Shizu_detectCompiler_Cpp(${name})
  Shizu_detectInstructionSetArchitecture(${name})
  Shizu_detectOperatingSystem(${name})
  Shizu_detectMultiTargetGenerator(${name})

endmacro()

# End an exexutable.
macro(Shizu_endExecutable)

  add_executable(${name} ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})

  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Includes")
  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Includes")

  Shizu_configureWarningsAndErrors(${name})
  source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})
  
  set_property(TARGET ${name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
  set_property(TARGET ${name} PROPERTY prerequisiteModules "")
  
  # We must define -rdynamic under Linux.
  if (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
    target_link_options(${name} PRIVATE -rdynamic)
  endif()

endmacro()

# Begin a dynamic library.
macro(Shizu_beginDynamicLibrary)

  set(${name}.source_files "")
  set(${name}.header_files "")
  set(${name}.configuration_files "")
  # Asset files are transitive.
  # plugin libraries and executables copy asset files to their output directory.
  set(${name}.asset_files "")

  Shizu_defineLanguages(${name})
  Shizu_detectCompiler_C(${name})
  Shizu_detectCompiler_Cpp(${name})
  Shizu_detectInstructionSetArchitecture(${name})
  Shizu_detectOperatingSystem(${name})
  Shizu_detectMultiTargetGenerator(${name})

endmacro()

# End a dynamic library.
macro(Shizu_endDynamicLibrary)

  add_library(${name} MODULE)

  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Includes")
  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Includes")

  target_sources(${name} PRIVATE ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})
  
  set_property(TARGET ${name} PROPERTY prerequisiteModules "")

  Shizu_configureWarningsAndErrors(${name})
  source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

endmacro()

function(on_plugins target)
  # Get the prerequisite modules of the target.
  get_target_property(prerequisiteModules ${target} prerequisiteModules)

  # Expand the prerequisite modules transitively.
  # For each prerequisite module A: Include any direct prerequisite
  set(CONTINUE TRUE)
  WHILE(CONTINUE)
    set(CONTINUE FALSE)
    get_target_property(xl ${target} prerequisiteModules)
    foreach (x ${xl})
        get_target_property(yl ${x} prerequisiteModules)
        foreach (y ${yl})
          if (NOT "${y}" IN_LIST xl)
            message(STATUS "adding ${y} to ${xl}")
            list(APPEND xl ${y})
            set(CONTINUE TRUE)
          endif()
        endforeach()
    endforeach()
    set_target_properties(${target} PROPERTIES prerequisiteModules "${xl}")
  ENDWHILE()

  set(commands "")
  set(targets "")
  set(sources "")
  set(messages "")
  get_target_property(xl ${target} prerequisiteModules)
  foreach (x ${xl})
    set(SOURCE_FILE $<TARGET_FILE:${x}>)
    set(TARGET_FILE $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_NAME:${x}>)
    list(APPEND sources ${SOURCE_FILE})
    list(APPEND targets ${TARGET_FILE})
    list(APPEND commands COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_FILE} ${TARGET_FILE})
    list(APPEND messages "[copy module ${target}]: copying ${SOURCE_FILE} to ${TARGET_FILE}\n")
  endforeach()
  # The target is effectively not executed if the dependencies (the module) is not modified.
  add_custom_target(
    "${target}.${module}.CopyModules"
    DEPENDS ${module}
    COMMAND ${commands}
    COMMENT ${messages}
  )
  add_dependencies(${target} "${target}.${module}.CopyModules")
endfunction()

function(on_assets target)
  get_target_property(source_dir ${target} SOURCE_DIR)
  message(STATUS "- [on_assets] ${target} source dir: ${source_dir}")

  file(GLOB_RECURSE assets_to_copy RELATIVE ${source_dir}/Assets ${source_dir}/Assets/*.* )
  message(STATUS "- [on_assets] ${target} assets: ${assets_to_copy}")

  set(commands "")
  set(targets "")
  set(sources "")
  set(messages "")
  foreach (asset_to_copy ${assets_to_copy})
    set(SOURCE_FILE ${source_dir}/Assets/${asset_to_copy})
    set(TARGET_FILE $<TARGET_FILE_DIR:${target}>/Assets/${asset_to_copy})
    list(APPEND sources ${SOURCE_FILE})
    list(APPEND targets ${TARGET_FILE})
    list(APPEND commands COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_FILE} ${TARGET_FILE})
    list(APPEND messages "[copy asset ${target}]: copying ${SOURCE_FILE} to ${TARGET_FILE}\n")
  endforeach()

  # The target is effectively not executed if the dependencies (the module) is not modified.
  add_custom_target(
    "${target}.CopyAssets"
    DEPENDS ${targets} ${sources}
    COMMAND ${commands}
    COMMENT ${messages}
  )
  add_dependencies(${target} "${target}.CopyAssets")
endfunction()

# Ensure all transitive dependencies of an interpreter executable copied to the executable directory.
function(on_executable target)
  on_plugins(${target})
  on_assets(${target})
endfunction()
