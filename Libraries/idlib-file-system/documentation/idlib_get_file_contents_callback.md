# idlib_get_file_contents_callback

## C Signature
````
bool (idlib_get_file_contents_callback)(void* context, void const* bytes, size_t number_of_bytes);
```

## Dscription
Type of a callback function that is invoked by `idlib_get_file_contents` when data was read.

## Parameters
- `context` The opaque context pointer as passed to `idlib_get_file_context`.
- `bytes` A pointer to an array of Bytes.
- `number_of_bytes` The number of Bytes in the array pointed to by `bytes`.

## Return value
Return `true` to continue and `false` to abort reading.
