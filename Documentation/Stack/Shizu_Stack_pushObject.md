# `Shizu_Stack_pushObject`

*Description*
Push a `Shizu_Reference(Shizu_Object)` object on the stack.

*Signature*
```
void
Shizu_Stack_pushObject
  (
    Shizu_State* state,
    Shizu_Reference(Shizu_Object) objectValue
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `objectValue` The `Shizu_Reference(Shizu_Object)` object to push.

*Errors*
- An allocation failed.
