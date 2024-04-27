# `Shizu_Stack_peek`

*Description*
Get the `Shizu_Value` object at the top of the stack.

*Signature*
```
Shizu_Value
Shizu_Stack_peek
  (
    Shizu_State* state
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.

*Error*
- the stack is empty

*Return Value*
The `Shizu_Value` object.
