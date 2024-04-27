# `Shizu_Stack_pushInteger32`

*Description*
Push a `Shizu_Integer32` object on the stack.

*Signature*
```
void
Shizu_Stack_pushInteger32
  (
    Shizu_State* state,
    Shizu_Integer32 integer32Value
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `integer32` The `Shizu_Integer32` object to push.

*Errors*
- An allocation failed.
