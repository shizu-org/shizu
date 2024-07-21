# Integrate with your CMake project
The proper way to integrate *IdLib File System* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib File System* and its dependency *IdLib Process*:

You can do that as follows:

```
include(FetchContent)

#######################################################################################################################

set(idlib-process.repository CACHE STRING "IdLib Process: The Git repository to check out")
if (NOT idlib-process.repository)
  set(idlib-process.repository https://github.com/michaelheilmann/idlib-process.git)
endif()

set(idlib-process.tag CACHE STRING "IdLib Process: The Git tag to check out")
if (NOT idlib-process.tag)
  set(idlib-process.tag 1.4)
endif()

set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-src")
endif()

set(idlib-process.binary-dir CACHE STRING "IdLib Process: Path to the build directory")
if (NOT idlib-process.binary-dir)
  set(idlib-process.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-bld")
endif()
                    
#######################################################################################################################

set(idlib-file-system.repository CACHE STRING "IdLib File System: The Git repository to check out")
if (NOT idlib-file-system.repository)
  set(idlib-file-system.repository https://github.com/michaelheilmann/idlib-file-system.git)
endif()

set(idlib-file-system.tag CACHE STRING "IdLib File System: The Git tag to check out")
if (NOT idlib-file-system.tag)
  set(idlib-file-system.tag 1.7)
endif()

set(idlib-file-system.source-dir CACHE STRING "IdLib File System: Path to the source directory")
if (NOT idlib-file-system.source-dir)
  set(idlib-file-system.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.7-src")
endif()

set(idlib-file-system.binary-dir CACHE STRING "IdLib File System: Path to the build directory")
if (NOT idlib-file-system.binary-dir)
  set(idlib-file-system.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.7-bld")
endif()

#######################################################################################################################

FetchContent_Declare(idlib-file-system
                     GIT_REPOSITORY ${idlib-file-system.repository}
                     GIT_TAG ${idlib-file-system.tag}
                     SOURCE_DIR ${idlib-file-system.source-dir}
                     BINARY_DIR ${idlib-file-system.binary-dir})

```

Now simply add
```
FetchContent_MakeAvailable(idlib-file-system)
```
And you can use *IdLib File System* by linking the library target `idlib-file-system` to a target `<target>`
```
target_link_libraries(<target> idlib-file-system)
```
and by adding the include directive  
```
#include "idlib/file_system.h"
```
in your C files.
