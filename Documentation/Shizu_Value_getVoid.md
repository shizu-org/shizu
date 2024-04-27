# `Shizu_Value_getVoid`

*Description*
Get the `Shizu_Void` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Void
Shizu_Value_getVoid
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Void` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing a `Shizu_Void` object.

*Return Value*
The `Shizu_Void` object stored in the `Shizu_Value` pointed to by `self`.
