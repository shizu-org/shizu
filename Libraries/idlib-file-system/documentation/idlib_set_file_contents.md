# `idlib_set_file_contents`

## C Signature
```
int
idlib_set_file_contents
  (
    char const* path_name,
    void const* p,
    size_t n
  );

int
idlib_set_file_contents_memory_mapped
  (
    char const* path_name,
    void const* p,
    size_t n
  )
```

## Description
This function sets the contents of a file.

`idlib_get_file_contents` and `idlib_get_file_contents_memory_mapped` both provide the same functionality, however, the former uses normal file reading functionality (ReadFile, read, ...) whilst the latter uses memory mapping.

## Parameters
- `path_name` A pointer to a C string. The path name of the file.
- `p` A pointer to an array of `n` Bytes.
- `n` The size, in Bytes, of the array pointed to by `p`. `0` is a valid size.

## Return value
`IDLIB_SUCCESS` on success. An error code on failure.
In particular, this function returns the following error codes on certain failures:
- `IDLIB_ARGUMENT_INVALID` `path_name` is a null pointer
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer
- `IDLIB_ALLOCATION_FAILED` an allocation failed
- `IDLIB_UNKNOWN_ERROR` an unknown error occurred

## Success
The contents of the file equal the contents of the memory block specified by `p` and `n`.
If the file did not exist, it was created.

## Failure
If this function fails, then it is undetermined if the file exists and if it exists what its contents are.
