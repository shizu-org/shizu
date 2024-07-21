# idlib_get_file_contents

## C Signature
```
int
idlib_get_file_contents
  (
    char const* path_name,
    void* context,
    idlib_get_file_contents_callback* callback
  );
int
idlib_get_file_contents_memory_mapped
  (
    char const* path_name,
    void* context,
    idlib_get_file_contents_callback* callback
  );
```

## Description
Get the contents of a file.

`idlib_get_file_contents` and `idlib_get_file_contents_memory_mapped` both provide the same functionality, however, the former uses normal file reading functionality (ReadFile, read, ...) whilst the latter uses memory mapping.

##Parameters
- `path_name` A pointer to a C string. The path name of the file.
- `context` An opaque context pointer passed to the callback.
- `callback` A pointer to a `idlib_get_file_contents_callback` function.

## Return values
Return `IDLIB_SUCCESS` on success. An error code on failure.
In particular, this function returns the following error codes on certain failures:
- `IDLIB_ARGUMENT_INVALID` `path_name` is a null pointer
- `IDLIB_ARGUMENT_INVALID` `callback` is a null pointer
- `IDLIB_ALLOCATION_FAILED` an allocation failed
- `IDLIB_UNKNOWN_ERROR` an unknown error occurred
