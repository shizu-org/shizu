# `Shizu_Value_setVoid`

*Description*
Store a `Shizu_Void` object in a `Shizu_Value` object/value.
This function can be used to initialize an `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setVoid
  (
    Shizu_Value* self,
    Shizu_Void voidValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to the `Shizu_Value` object/value to store the `Shizu_Void` object in.
- `voidValue` The `Shizu_Void` object to store.
