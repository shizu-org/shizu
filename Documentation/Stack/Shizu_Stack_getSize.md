# `Shizu_Stack_getSize`

*Description*
Get the size of the stack.

*Signature*
```
size_t
Shizu_Stack_getSize
  (
    Shizu_State* state
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.

*Return Value*
The size of the stack.
