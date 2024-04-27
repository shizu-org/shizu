# `Shizu_Stack_pop`

*Description*
Pop the top of the stack.

*Signature*
```
Shizu_Stack_pop
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