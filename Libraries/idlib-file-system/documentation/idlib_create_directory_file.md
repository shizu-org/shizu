# idlib_create_directory_file

## C Signature
```
int
idlib_create_directory_file
  (
    char const* path_name
  );
```

## Description
Create a directory file.
The directories up to the directory to be created must exist.

## Parameters
- `path_name` A pointer to a C string. The path name of the directory to create.

## Return value
`IDLIB_SUCCESS` on success. A non-zero value on failure.
This function may use the following values to indicate a particular cause of an error:
- `IDLIB_ARGUMENT_INVALID` `path_name` is a null pointer.
- `IDLIB_UNKNOWN_ERROR` An unknown error occurred.

## Success
The directory exists.
