# `Shizu_Value_isVoid`

*Description*
Get if a `Shizu_Value` object holds a `Shizu_Void` object.

*Signature*
```
bool
Shizu_Value_isVoid
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object.

*Return Value*
`true` if the `Shizu_Value` object holds a `Shizu_Void` object. `false` otherwise.
