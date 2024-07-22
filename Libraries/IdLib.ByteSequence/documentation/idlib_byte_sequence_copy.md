# `idlib_byte_sequence_copy`

## C Signature
```
idlib_status
idlib_byte_sequence_copy
  (
    void* p,
    void const* q,
    size_t n
  );
```

## Description
Copy the contents of the memory block denoted by `(q,n)` over the contents of the memory block denoted by `(p,n)`.
The memory blocks may not overlap.

## Parameters
- `p` A pointer to the target memory block.
- `q` A pointer to the source memory block.
- `n` The size, in elements, of the memory blocks.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `q` is a null pointer.
