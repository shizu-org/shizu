# `Shizu_State_getStatus`

*Description*
Get the value of the status variable of a `Shizu_State` singleton value.

*Signature*
```
Shizu_Status
Shizu_State_getStatus
  (
    Shizu_State* SELF
  );
```

*Details*
The initial value of the status variable of a `Shizu_State` object is `Shizu_Status_Nominal`.

*Parameters*
- `SELF` A pointer to a `Shizu_State` object.

*Return Value*
the value of the status variable of the `Shizu_State` object pointed to by `SELF`.
