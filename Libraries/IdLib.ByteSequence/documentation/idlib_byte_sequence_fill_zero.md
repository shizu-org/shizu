# `idlib_byte_sequence_fill_zero`

## C Signature
```
idlib_status
idlib_byte_sequence_fill_zero
  (
    void* p,
    size_t n
  );
```

## Description
Assign each element of a memory block `(p,n)` the zero value.

## Parameters
- `p` A pointer to an array of Bytes of length `n`.
- `n` The size of the array pointed to by `p`.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
