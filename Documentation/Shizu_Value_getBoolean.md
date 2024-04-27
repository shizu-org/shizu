# `Shizu_Value_getBoolean`

*Description*
Get the `Shizu_Boolean` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Boolean
Shizu_Value_getBoolean
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Boolean` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing a `Shizu_Boolean` object.

*Return Value*
The `Shizu_Boolean` object stored in the `Shizu_Value` pointed to by `self`.
