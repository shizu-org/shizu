# `idlib_byte_sequence_find`

## C Signature
```
idlib_status
idlib_byte_sequence_find
  (
    void const* p,
    size_t n,
    void const* q,
    size_t m,
    bool* found,
    size_t* index
  );
```

## Description
Find the first occurrence of a value `v` in a Byte sequence `(p,n)`.

## Parameters
- `RETURN` A pointer to a `size_t` variable.
- `p` A pointer to an array of Bytes of length `n`.
- `n` The length of the array pointed to by `p`.
- `q` A pointer o an array of Bytes of length `m`.
- `m` The length of the array pointed to by `q`.
- `found` A pointer to a `bool` variable.
- `index` A pointer to a `size_t` variable.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `q` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `found` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `index` is a null pointer.

## Success
If `(q,m)` was found in `(p,n)`:
- `*found` was assigned `true`
- `*index` was assigned the index of the leftmost occurrence of `(q,m)` in `(p,n)`

If `(q,m)` was not found `(p,n)`:
- `*found` was assigned `false`
- `*index` was assigned `m`
