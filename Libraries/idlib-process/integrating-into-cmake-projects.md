# Integrate with your CMake project
The proper way to integrate *IdLib Process* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Process*:

You can do that as follows:

```
add_subdirectory(<subdirectory for IdLib Process>)
```

And you can use *IdLib Process* by linking the library target `idlib-process` to a target `<target>`
```
target_link_libraries(<target> idlib-process)
```
and by adding the include directive  
```
#include "idlib/process.h"
```
in your C files.
