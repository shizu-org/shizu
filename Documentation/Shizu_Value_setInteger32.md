# `Shizu_Value_setInteger32`

*Description*
Store a `Shizu_Integer32` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setInteger32
  (
    Shizu_Value* self,
    Shizu_Integer32 integer32Value
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to the `Shizu_Value` object/value to store the `Shizu_Integer32` in.
- `integer32Value` The `Shizu_Integer32` object to store.
