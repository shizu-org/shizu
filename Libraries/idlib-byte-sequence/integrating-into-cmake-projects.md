# Integrate with your CMake project
The proper way to integrate *IdLib Byte Sequence* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Byte Sequence* and its dependency *IdLib Process*:

You can do that as follows:

First, ensure that the paths to the dependencies are known:
```
if (NOT ${idlib-process.source-dir})
  set(idlib-process.source-dir <path to source directory of IdLib Process>)
endif()
```

Second, add the subdirectory of *IdLib Byte Sequence*:
```
add_subdirectory(<subdirectory for IdLib Byte Sequence>)
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
