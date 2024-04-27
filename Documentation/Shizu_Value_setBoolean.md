# `Shizu_Value_setBoolean`

*Description*
Store a `Shizu_Boolean` value in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setBoolean
  (
    Shizu_Value* self,
    Shizu_Boolean booleanValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to a `Shizu_Value` object/value to store the `Shizu_Boolean` object in.
- `booleanValue` The `Shizu_Boolean` object to store.
