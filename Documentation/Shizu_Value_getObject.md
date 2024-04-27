# `Shizu_Value_getObject`

*Description*
Get the `Shizu_Reference(Shizu_Object)` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Reference(Shizu_Object)
Shizu_Value_getObject
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Reference(Shizu_Object)` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing a `Shizu_Object` object.

*Return Value*
The `Shizu_Reference(Shizu_Object)` object stored in the `Shizu_Value` pointed to by `self`.
