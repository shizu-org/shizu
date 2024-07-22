# `idlib_byte_sequence_find_value`

## C Signature
```
idlib_status
idlib_byte_sequence_find_value
  (
    void const* p,
    size_t n,
    uint8_t v,
    bool* found,
    size_t* index
  );
```

## Description
Find the first occurrence of a value `v` in a Byte sequence `(p,n)`.

## Parameters
- `RETURN` A pointer to a `size_t` variable.
- `p` A pointer to an array of Bytes of length `n`.
- `n` The size of the array pointed to by `p`.
- `found` A pointer to a `bool` variable.
- `index` A pointert to a `size_t` variable.

## Return values
`IDLIB_SUCCESS` on success.
The following error status codes are returned on failure:
- `IDLIB_ARGUMENT_INVALID` `p` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `found` is a null pointer.
- `IDLIB_ARGUMENT_INVALID` `index` is a null pointer.

## Success
If `v` was found in `(p,n)`:
- `*found` was assigned `true`
- `*index` was assigned the index of the leftmost occurrence of `v` in `(p,n)`

If `v` was not found `(p,n)`:
- `*found` was assigned `false`
- `*index` was assigned `m`
