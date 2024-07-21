# Integrate with your CMake project
The proper way to integrate *IdLib File System* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib File System* and its dependency *IdLib Process*:

You can do that as follows:

First, ensure that the paths to the dependencies are known.
```
if (NOT ${idlib-process.source-dir})
  set(idlib-process.source-dir <path to source directory of IdLib Process>)
endif()
```

Second, add the subdirectory of *IdLib File System*:
```
add_subdirectory(<subdirectory for IdLib File System>)
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
