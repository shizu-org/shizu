# `idlib_byte_sequence_compare`

## C Signature
```
idlib_status
idlib_byte_sequence_compare
  (
    int8_t* RETURN,
    void* p,
    void const* q,
    size_t n
  );
```

## Description
Compare the memory blocks `(p,n)` and `(q,n)`.
Assign `*RETURN` an integer value depending on if `p` is smaller, equal, or greater than `q`:
- `0`  if the memory block `(p,n)` and the memory block `(q,n)` are equal
- `-1` if the memory block `(p,n)` is smaller than the memory block denoted by `(q,n)`
- `+1` if the memory block `(p,n)` is greater than the memory block denoted by `(q,n)`

The memory blocks are equal if all element values are equal.
If there are two corresponding elements that are not equal, then the first (from the left) corresponding elements that are not equal are compared:
- if the element in `(p,n)` is smaller than the element in `(q,n)` then `(p,n)` is smaller than `(q,n)`
- if the element in `(p,n)` is greater than the element in `(q,n)` then `(p,n)` is greater than `(q,n)`

## Parameters
- `p` A pointer to a memory block `(p,n)`.
- `q` A pointer to a memory block `(q,n)`.
- `n` The number of Bytes to copy.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `q` is a null pointer.