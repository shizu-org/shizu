# `Shizu_Stack_push`

*Description*
Push a `Shizu_Value` object on the top of the stack.

*Signature*
```
void
Shizu_Stack_push
  (
    Shizu_State* state,
    Shizu_Value value
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `value` The `Shizu_Value` object to push.

*Error*
- an allocation failed
