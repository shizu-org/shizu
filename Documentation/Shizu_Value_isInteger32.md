# `Shizu_Value_isInteger32`

*Description*
Get if a `Shizu_Value` object holds a `Shizu_Integer32` object.

*Signature*
```
bool
Shizu_Value_isInteger32
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object.

*Return Value*
`true` if the `Shizu_Value` object holds a `Shizu_Integer32` object. `false` otherwise.
