# `Shizu_Value_getFloat32`

*Description*
Get the `Shizu_Float32` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Float32
Shizu_Value_getFloat32
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Float32` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing a `Shizu_Float32` object.

*Return Value*
The `Shizu_Float32` object stored in the `Shizu_Value` object pointed to by `self`.
