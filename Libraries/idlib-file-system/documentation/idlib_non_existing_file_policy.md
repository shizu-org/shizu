# idlib_non_existing_file_policy

## C Signature
````
typedef enum <implementation { <elements> } idlib_non_existing_file_policy;
```

## Description
An enumeration of policies for opening a file that does not exist.
The enumeration elements cannot be OR'ed together.

## Elements
- `idlib_file_access_mode_read` Read access. Can be combined with `idlib_file_access_mode_write`.
- `idlib_file_access_mode_write` Write access. Can be combined with `idlib_file_access_mode_read`.
- `idlib_file_access_mode_read_write` Read and write access.
   Alias for `idlib_file_access_mode_read|idlib_file_access_mode_write` and `idlib_file_access_mode_write_read`.
- `idlib_file_access_mode_write_read` Write and read access.
   Alias for `idlib_file_access_mode_write|idlib_file_access_mode_read` and `idlib_file_access_mode_read_write`.
