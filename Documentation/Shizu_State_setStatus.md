# `Shizu_State_setStatus`

*Description*
Set the value of the status variable of a `Shizu_State` object.

*Signature*
```
void
Shizu_State_setStatus
  (
    Shizu_State* SELF,
    Shizu_Status status
  );
```

*Details*
The initial value of the status variable of a `Shizu_State` object is `Shizu_Status_NoError`.

*Parameters*
- `SELF` A pointer to a `Shizu_State` value.
- `status` The value to assign to the status variable of the `Shizu_State` value.
