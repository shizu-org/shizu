# `Shizu_State2_pushJumpTarget`

*Description*
Pop a jump target on the stack of jump targets.

*Signature*
```
void
Shizu_State2_popJumpTarget
  (
    Shizu_State2* SELF
  );
```

*Remarks*
The behavior of this function is undefined if at least one of the following conditions holds:
- `SELF` does not point to a `Shizu_State2`
- the jump target stack is empty

*Parameters*
- `SELF` A pointer to a `Shizu_State2` object.
