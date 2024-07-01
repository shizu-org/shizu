# `Shizu_Stack_getSize`

*Description*
Get the size of the stack.

*Signature*
```
size_t
Shizu_Stack_getSize
  (
    Shizu_State2* state
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State2` object.

*Parameters*
- `state` A pointer to a `Shizu_State2` object.

*Return Value*
The size of the stack.
