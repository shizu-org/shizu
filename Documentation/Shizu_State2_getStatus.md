# `Shizu_State2_getStatus`

*Description*
Get the value of the status variable of a `Shizu_State2` singleton value.

*Signature*
```
Shizu_Status
Shizu_State2_getStatus
  (
    Shizu_State2* SELF
  );
```

*Details*
The initial value of the status variable of a `Shizu_State2` object is `Shizu_Status_Success`.

*Parameters*
- `SELF` A pointer to a `Shizu_State2` object.

*Return Value*
the value of the status variable of the `Shizu_State2` object pointed to by `SELF`.
