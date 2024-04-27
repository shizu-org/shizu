# `Shizu_State_destroy`

*Description*
Relinquish a reference to the `Shizu_State` singleton object.

*Signature*
```
int
Shizu_State_destroy
  (
    Shizu_State* SELF
  );
```
*Parameters*
- `SELF` A pointer to a `Shizu_State` object.

*Return Value`*
A zero value on success, a non-zero value on failure.

*Details*
See [Shize_State_destroy](Shizu_State_destroy.md) for more information.
Multiple calls to `Shizu_State_create` increment the reference count of that singleton object.

*Success*
The `Shizu_State` object pointed to by `SELF` was destroyed.

*Failure*
The environment was not changed.
