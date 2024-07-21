# Integrate with your CMake project
The proper way to integrate *IdLib Bit Scan* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Bit Scan* and its dependency *IdLib Process*:

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
  set(idlib-process.tag 1.2)
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

set(idlib-bit-scan.repository CACHE STRING "IdLib Bit Scan: The Git repository to check out")
if (NOT idlib-bit-scan.repository)
  set(idlib-bit-scan.repository https://github.com/michaelheilmann/idlib-bit-scan.git)
endif()

set(idlib-bit-scan.tag CACHE STRING "IdLib Bit Scan: The Git tag to check out")
if (NOT idlib-bit-scan.tag)
  set(idlib-bit-scan.tag 1.4)
endif()

set(idlib-bit-scan.source-dir CACHE STRING "IdLib Bit Scan: Path to the source directory")
if (NOT idlib-bit-scan.source-dir)
  set(idlib-bit-scan.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.4-src")
endif()

set(idlib-bit-scan.binary-dir CACHE STRING "IdLib File System: Path to the build directory")
if (NOT idlib-bit-scan.binary-dir)
  set(idlib-bit-scan.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.4-bld")
endif()

#######################################################################################################################

FetchContent_Declare(idlib-bit-scan
                     GIT_REPOSITORY ${idlib-bit-scan.repository}
                     GIT_TAG ${idlib-bit-scan.tag}
                     SOURCE_DIR ${idlib-bit-scan.source-dir}
                     BINARY_DIR ${idlib-bit-scan.binary-dir})

```

Now simply add
```
FetchContent_MakeAvailable(idlib-bit-scan)
```
And you can use *IdLib Bit Scan* by linking the library target `idlib-bit-scan` to a target `<target>`
```
target_link_libraries(<target> idlib-bit-scan)
```
and by adding the include directive  
```
#include "idlib/bit_scan.h"
```
in your C files.
