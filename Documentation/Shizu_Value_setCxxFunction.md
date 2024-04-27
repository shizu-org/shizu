# `Shizu_Value_setCxxFunction`

*Description*
Store a `Shizu_Reference(Shizu_CxxFunction)` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setCxxFunction
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to the `Shizu_Value` object/value to store the `Shizu_Reference(Shizu_CxxFunction)*` object in.
- `cxxFunctionValue` The `Shizu_Reference(Shizu_CxxFunction)` object to store.
