# idlib_file_handle_create

## C Signature
```
int
idlib_file_handle_create
  (
    idlib_file_handle** file_handle,
    char const* pathname,
    idlib_file_access_mode file_access_mode,
    idlib_existing_file_policy existing_file_policy,
    idllib_non_existing_file_policy non_existing_file_policy
  );
```

## Description
Create a file handle.

## Parameters
- `file_handle` A pointer to an `idlib_file_handle*` variable.
- `path_name` A pointer to a C string. That string denotes the path of the file to create the file handle for.
- `file_access_mode` The file access mode for opening the file.
- `existing_file_policy`The policy to apply if the opened file exists.
- `non_existing_file_policy` The policy to apply if the opened file does not exist.

## Return value
`IDLIB_SUCCESS` on success. `IDLIB_FAILURE` on failure.

## Success**
`*file_handle` was assigned a pointer to a `idlib_file_handle` object.
The caller acquired a reference to that object.

## Failure**Failure**
`file_handle` was not dereferenced.
