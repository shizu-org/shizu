# `Shizu_State_pushJumpTarget`

*Description*
Push a jump target on the stack of jump targets.

*Signature*
```
void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  );
```

*Remarks*
The behavior of this function is undefined if at least one of the following conditions holds:
- `self` does not point to a Shizu_State object
- `jumpTarget` does not point to a Shizu_JumpTarget object

*Parameters*
- `self` A pointer to a `Shizu_State` object.
- `jumpTarget` A pointer to a `Shizu_JumpTarget` object.
