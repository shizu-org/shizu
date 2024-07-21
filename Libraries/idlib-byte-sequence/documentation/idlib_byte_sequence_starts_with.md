# `idlib_byte_sequence_starts_with`

## C Signature
```
idlib_status
idlib_byte_sequence_starts_with
  (
    bool* RETURN,
    void const* p,
    size_t n,
    void const* q,
    size_t m    
  );
```

## Description
Find the first occurrence of a value `v` in a Byte sequence `(p,n)`.

## Parameters
- `RETURN` A pointer to a `bool` variable.
- `p` A pointer to an array of Bytes of length `n`.
- `n` The size of the array pointed to by `p`.
- `q` A pointert o an array of Bytes of length `m`.
- `m` The length of the array pointed to by `q`.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `n` is greater than `SIZE_MAX - 2`.
- `IDLIB_ARGUMENT_INVALID` `q` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `m` is greater than `SIZE_MAX - 2`.

## Success
`*RETURN` was assigned `true` if the Byte sequence `(p,n)` starts with the Byte sequence `(q,m)`.
Otherwise it was assigned `false`.
