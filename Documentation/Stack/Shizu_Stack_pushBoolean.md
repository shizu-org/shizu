# `Shizu_Stack_pushBoolean`

*Description*
Push a `Shizu_Boolean` object on the stack.

*Signature*
```
void
Shizu_Stack_pushBoolean
  (
    Shizu_State* state,
    Shizu_Boolean booleanValue
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `booleanValue` The `Shizu_Boolean` object to push.

*Errors*
- An allocation failed.
