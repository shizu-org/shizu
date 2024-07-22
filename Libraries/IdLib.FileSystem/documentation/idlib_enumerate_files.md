# idlib_enumerate_files

## C Signature
```
typedef void idlib_enumerate_files_callback_context;
typedef bool (idlib_enumerate_files_callback_function)(idlib_get_working_directory_callback_context* context, void const* p, size_t n);

int
idlib_enumerate_files
  (
    char const* path,
    idlib_enumerate_files_callback_context* callback_context,
    idlib_enumerate_files_callback_function* callback_function,
    bool regular_files,
    bool directory_files
  );
```

## Description
Enumerate the files in a directory.

This function enumerates the files in the directory specified by `path`.
It invokes `callback_function` with `callback_context` as the first argument.
The second and third argument receive a pointer to the Bytes / the number of Bytes of the unqualified filename found.

##Parameters
- `path` A pointer to a C string. The path name of the file.
- `callback_context` An opaque pointer passed to the callback function. The pointer is opaque, that is, it may be any pointer value including the null pointer.
- `callback_function` A pointer to a `idlib_enumerate_files_callback_function` function.

## Return values
Return `IDLIB_SUCCESS` on success.
An error code on failure.
In particular, this function returns the following error codes on certain failures:
- `IDLIB_ARGUMENT_INVALID` `path` is a null pointer
- `IDLIB_ARGUMENT_INVALID` `callback_function` is a null pointer
- `IDLIB_ALLOCATION_FAILED` an allocation failed
- `IDLIB_ENVIRONMENT_FAILED` the environment failed
- `IDLIB_ABORTED` `callback_function` returned `false`
Other error codes may be returned.