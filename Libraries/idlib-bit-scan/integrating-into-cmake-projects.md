# Integrate with your CMake project
The proper way to integrate *IdLib Bit Scan* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Bit Scan* and its dependency *IdLib Process*:

You can do that as follows:

First, ensure that the paths to the dependencies are known:
```
if (NOT ${idlib-process.source-dir})
  set(idlib-process.source-dir <path to source directory of IdLib Process>)
endif()
```

Second, add the subdirectory of *IdLib Bit Scan*:
```
add_subdirectory(<subdirectory for IdLib Bit Scan>)
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
