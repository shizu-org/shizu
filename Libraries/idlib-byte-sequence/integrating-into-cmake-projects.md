# Integrate with your CMake project
The proper way to integrate *IdLib Byte Sequence* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Byte Sequence* and its dependency *IdLib Process*:

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

set(idlib-byte-sequence.repository CACHE STRING "IdLib Byte Sequence: The Git repository to check out")
if (NOT idlib-byte-sequence.repository)
  set(idlib-byte-sequence.repository https://github.com/michaelheilmann/idlib-byte-sequence.git)
endif()

set(idlib-byte-sequence.tag CACHE STRING "IdLib Byte Sequence: The Git tag to check out")
if (NOT idlib-byte-sequence.tag)
  set(idlib-byte-sequence.tag 1.0)
endif()

set(idlib-byte-sequence.source-dir CACHE STRING "IdLib Byte Sequence: Path to the source directory")
if (NOT idlib-byte-sequence.source-dir)
  set(idlib-byte-sequence.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-byte-sequence-1.0-src")
endif()

set(idlib-byte-sequence.binary-dir CACHE STRING "IdLib Byte Sequence: Path to the build directory")
if (NOT idlib-byte-sequence.binary-dir)
  set(idlib-byte-sequence.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-byte-sequence-1.0-bld")
endif()

#######################################################################################################################

FetchContent_Declare(idlib-byte-sequence
                     GIT_REPOSITORY ${idlib-byte-sequence.repository}
                     GIT_TAG ${idlib-byte-sequence.tag}
                     SOURCE_DIR ${idlib-byte-sequence.source-dir}
                     BINARY_DIR ${idlib-byte-sequence.binary-dir})

```

Now simply add
```
FetchContent_MakeAvailable(idlib-byte-sequence)
```
And you can use *IdLib Byte Sequence* by linking the library target `idlib-byte-sequence` to a target `<target>`
```
target_link_libraries(<target> idlib-byte-sequence)
```
and by adding the include directive  
```
#include "idlib/byte_sequence.h"
```
in your C files.
