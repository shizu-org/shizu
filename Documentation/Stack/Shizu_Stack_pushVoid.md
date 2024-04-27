# `Shizu_Stack_pushVoid`

*Description*
Push a `Shizu_Void` object on the stack.

*Signature*
```
void
Shizu_Stack_pushVoid
  (
    Shizu_State* state,
    Shizu_Void voidValue
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `voidValue` The `Shizu_Void` object to push.

*Errors*
- An allocation failed.
