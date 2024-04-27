# `Shizu_Stack_isCxxFunction`

*Description*
Get the object at the specified index on the stack is a `Shizu_Reference(Shizu_CxxFunction)` object.

*Signature*
```
bool
Shizu_Stack_isCxxFunction
  (
    Shizu_State* state,
    size_t index
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `self` A pointer to a `Shizu_State` object.
- `index` The index.

*Errors*
- `index` is greater than or equal to the size of the stack.

*Return Value*
`true` if the object at the specified index is a `Shizu_Reference(Shizu_CxxFunction)` object. `false` otherwise.