# `Shizu_Value_setObject`

*Description*
Store a `Shizu_Reference(Shizu_Object)` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setObject
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_Object) objectValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to the `Shizu_Value` object/value to store the `Shizu_Reference(Shizu_Object)` object in.
- `objectValue` A pointer to the `Shizu_Object` object to store.
