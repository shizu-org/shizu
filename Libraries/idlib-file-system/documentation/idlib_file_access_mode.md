# idlib_file_access_mode

## C Signature
````
typedef enum <implementation { <elements> } idlib_file_access_mode;
```

## Description
An enumeration of file access modes. The enumeration elements can be OR'ed together.

## Elements
- `idlib_file_access_mode_read` Read access. Can be combined with `idlib_file_access_mode_write`.
- `idlib_file_access_mode_write` Write access. Can be combined with `idlib_file_access_mode_read`.
- `idlib_file_access_mode_read_write` Read and write access.
   Alias for `idlib_file_access_mode_read|idlib_file_access_mode_write` and `idlib_file_access_mode_write_read`.
- `idlib_file_access_mode_write_read` Write and read access.
   Alias for `idlib_file_access_mode_write|idlib_file_access_mode_read` and `idlib_file_access_mode_read_write`.
