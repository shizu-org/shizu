# `idlib_byte_sequence_fill_value`

## C Signature
```
idlib_status
idlib_byte_sequence_fill
  (
    void* p,
    size_t n,
    uint8_t v
  );
```

## Description
Assign each element of a Byte sequence `(p,n)` the specified value.

## Parameters
- `p` A pointer to an array of Bytes of length `n`.
- `n` The length of the array pointed to by `p`.
- `v` The value to assign to each element in the array.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
