# `Shizu_String_create`

*Description*
Create a `Shizu_String` object.

*Signature*
```
Shizu_String*
Shizu_String_create
  (
    Shizu_State2* state,
    void const* bytes,
    size_t numberOfBytes
  );
```

*Details*
Create  a `Shizu_String` object from the specified array of Bytes.
This function checks if the Bytes are a valid UTF8 string.

*Parameters*
- `state` A pointer to a `Shizu_State2` object.
- `bytes` A pointer to an array of Bytes.
- `numberOfBytes` The number of Bytes in the array pointed to by `bytes`.

*Error*
- `Shizu_Status_InvalidArgument` `bytes` is a null pointer
- `Shizu_Status_InvalidEncoding` The Bytes are not a valid UTF8 string.
