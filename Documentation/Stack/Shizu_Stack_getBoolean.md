# `Shizu_Stack_getBoolean`

*Description*
Get the `Shizu_Boolean` object stored at a stack index.

*Signature*
```
Shizu_Boolean
Shizu_Stack_getBoolean
  (
    Shizu_State* state,
    size_t index
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to `Shizu_State` object.

*Parameters*
- `self` A pointer to a `Shizu_State` object.
- `index` The stack index.

*Error*
- the stack index greater than or equal to the stack size
- the object stored at the stack index is not a `Shizu_Boolean` object

*Return Value*
The `Shizu_Boolean` object.
