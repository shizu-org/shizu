# `Shizu_State2_pushJumpTarget`

*Description*
Push a jump target on the stack of jump targets.

*Signature*
```
void
Shizu_State2_pushJumpTarget
  (
    Shizu_State2* SELF,
    Shizu_JumpTarget* jumpTarget
  );
```

*Remarks*
The behavior of this function is undefined if at least one of the following conditions holds:
- `SELF` does not point to a Shizu_State object
- `jumpTarget` does not point to a Shizu_JumpTarget object

*Parameters*
- `SELF` A pointer to a `Shizu_State2` object.
- `jumpTarget` A pointer to a `Shizu_JumpTarget` object.
