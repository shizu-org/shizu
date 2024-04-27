# `Shizu_State_pushJumpTarget`

*Description*
Pop a jump target on the stack of jump targets.

*Signature*
```
void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  );
```

*Remarks*
The behavior of this function is undefined if at least one of the following conditions holds:
- `self` does not point to a `Shizu_State`
- the jump target stack is empty

*Parameters*
- `self` A pointer to a `Shizu_State` object.
