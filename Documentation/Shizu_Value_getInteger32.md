# `Shizu_Value_getInteger32`

*Description*
Get the `Shizu_Integer32` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Integer32
Shizu_Value_getInteger32
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Integer32` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing `Shizu_Integer32` object.

*Return Value*
The `Shizu_Integer32` object stored in the `Shizu_Value` pointed to by `self`.
