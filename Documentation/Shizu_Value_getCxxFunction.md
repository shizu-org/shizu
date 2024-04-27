# `Shizu_Value_getCxxFunction`

*Description*
Get the `Shizu_Reference(Shizu_CxxFunction)` object stored in a `Shizu_Value` object.

*Signature*
```
Shizu_Reference(Shizu_CxxFunction)
Shizu_Value_getCxxFunction
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object that is storing a `Shizu_Reference(Shizu_CxxFunction)` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object that is storing a `Shizu_Reference(Shizu_CxxFunction)` object.

*Return Value*
A pointer to the `Shizu_Reference(Shizu_CxxFunction)` object stored in the `Shizu_Value` object pointed to by `self`.
