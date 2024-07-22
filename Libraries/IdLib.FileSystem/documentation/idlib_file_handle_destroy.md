# idlib_file_handle_destroy

## C Signature
```
int
idlib_file_handle_destroy
  (
    idlib_file_handle *file_handle
  );
```

## Description
Destroy a file handle.

## Parameters
- `file_handle` A pointer to an `idlib_file_handle` object previously created by a successful call to `idlib_file_handle_create`.

## Return value
`IDLIB_SUCCESS` on success. `IDLIB_FAILURE`` on failure.

## Success
The `idlib_file_handle` object pointed to by `file_handle` was destroyed.

## Failure
`file_handle` was not dereferenced.
