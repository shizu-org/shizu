# idlib_delete_file

## C Signature
```
int
idlib_delete_file
  (
    char const* path_name
  );
```

## Description
Delete a file.
If the file to delete is a directory file, then it must be empty.

## Parameters
- `path_name` A pointer to a C string. The path name of the file.

## Return value
`IDLIB_SUCCESS` on success. A non-zero value on failure.
In particular, this function may return the following error codes
- `IDLIB_INVALID_ARGUMENT` `path_name´ is a null pointer
- `IDLIB_INVALID_PATH_NAME` The C string pointed to by `path_name` is not a valid path name.
- `IDLIB_DIRECTORY_NOT_EMPTY` The file to delete is a directory file that is not empty.
