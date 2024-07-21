# idlib_get_working_directory

## C Signature
```
typedef void idlib_get_working_directory_callback_context;
typedef bool (idlib_get_working_directory_callback_function)(idlib_get_working_directory_callback_context* context, void const* p, size_t n);

int
idlib_get_working_directory
  (
    idlib_get_working_directory_callback_context* callback_context,
    idlib_get_working_directory_callback_function* callback_function
  );
```

## Description
Get the working directory.

It invokes `callback_function` with `callback_context` as the first argument.
The second and third argument receive a pointer to the Bytes / the number of Bytes of the absolute working directory.

##Parameters
- `path_name` A pointer to a C string. The path name of the file.
- `callback_context` An opaque pointer passed to the callback function. The pointer is opaque, that is, it may be any pointer value including the null pointer.
- `callback_function` A pointer to a `idlib_get_working_directory_callback_function` function.

## Return values
Return `IDLIB_SUCCESS` on success. One of the following error codes is returned on failure:
- `IDLIB_ARGUMENT_INVALID` `callback_function` is a null pointer
- `IDLIB_ALLOCATION_FAILED` an allocation failed
- `IDLIB_ENVIRONMENT_FAILED` the environment failed
- `IDLIB_ABORTED` `callback_function` returned `false`
No other error codes are returned.
