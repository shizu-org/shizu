# `Shizu_Value_setFloat32`

*Description*
Store a `Shizu_Float32` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setFloat32
  (
    Shizu_Value* self,
    Shizu_Float32 float32Value
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to the `Shizu_Value` object/value to store the `Shizu_Float32` object in.
- `float32Value` The `Shizu_Float32` object to store.
