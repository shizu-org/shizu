# `Shizu_Stack_getCxxFunction`

*Description*
Get the `Shizu_Reference(Shizu_CxxFunction)` object on top of the stack.

*Signature*
```
Shizu_Reference(Shizu_CxxFunction)
Shizu_Stack_peekCxxFunction
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
- the object stored at the top of the stack is not a `Shizu_Reference(Shizu_CxxFunction)` object

*Return Value*
The `Shizu_Reference(Shizu_CxxFunction)` object.
