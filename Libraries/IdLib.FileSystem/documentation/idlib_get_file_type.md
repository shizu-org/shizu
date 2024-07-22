# `idlib_get_file_type`

## C Signature
```
int
idlib_get_file_type
  (
    char const* path_name,
    idlib_file_type* file_type
  );
```

## Description
This function gets the type of a file.

If this function succeeds it returns `IDLIB_SUCCESS` and `*fileState` was assigned a `Nucleus_FileState` value.

This function fails if and only if one of the following conditions is true_
- `path_name` or `file_statze` are null pointers or the pathname pointed to by `path_name` is not a valid pathname.
   `IDLIB_ARGUMENT_INVALID` is returned in these cases.
- The environment fails.
  `IDLIB_ENVIRONMENT_FAILED` is returned in this case.
- An allocation fails.
  `IDLIB_ALLOCATION_FAILED` is returned in this case.

## Return value
`IDLIB_SUCCESS` on success.
- `IDLIB_ARGUMENT_INVALID` `path_name` is a null pointer or is not a valid path
- `IDLIB_ARGUMENT_INVALID` `file_state` is a null pointer
- `IDLIB_FILE_NOT_FOUND` the file was not found
- `IDLIB_UNKNOWN_ERROR` an unknown error occurred

## Success
If this function succeeds it returns `IDLIB_SUCCESS` and `*fileState` was assigned a `idlib_file_state` value.

## Failure
`*file_type` was not dereferenced.

## Parameters
- `path_name` a pointer to a path name string
- `file_state` a pointer to an `idlib_file_state` variable
