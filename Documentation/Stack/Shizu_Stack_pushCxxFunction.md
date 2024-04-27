# `Shizu_Stack_pushCxxFunction`

*Description*
Push a `Shizu_Reference(Shizu_CxxFunction)` object on the stack.

*Signature*
```
void
Shizu_Stack_pushCxxFunction
  (
    Shizu_State* state,
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue
  );
```

*Remarks*
The behaviour of this function is undefined if `state` does not point to a `Shizu_State` object.

*Parameters*
- `state` A pointer to a `Shizu_State` object.
- `cxxFunctionValue` The `Shizu_Reference(Shizu_CxxFunction)` object to push.

*Errors*
- An allocation failed.
