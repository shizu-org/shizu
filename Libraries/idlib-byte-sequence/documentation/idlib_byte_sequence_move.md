# `idlib_byte_sequence_move`

## C Signature
```
idlib_status
idlib_byte_sequence_move
  (
    void* p,
    void const* q,
    size_t n
  );
```

## Description
Copy the contents of the memory block denoted by `(q,n)` over the contents of the memory block denoted by `(p,n)`.
The memory blocks may overlap.

## Parameters
- `p` A pointer to an array of Bytes of length `n`.
- `q` A pointer to an array of Bytes of length `n`.
- `n` The size, in elements, of the arrays pointed to by `p` and `q`, respectively.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `q` is a null pointer.
