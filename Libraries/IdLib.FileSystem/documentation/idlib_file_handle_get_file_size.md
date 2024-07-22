# idlib_file_handle_get_file_size

## C Signature
```
int
idlib_file_handle_get_size
  (
    idlib_file_handle* file_handle,
    size_t* file_size
  );
```

## Description
Get the size, in Bytes, of a file.

## Parameters
- `file_handle` A pointer to an `idlib_file_handle` object.
- `file_size` A pointer to a `size_t` variable.

## Success
`*file_size` was assigned the size, in Bytes, of the file.

## Failure
`file_size` was not dereferenced.
