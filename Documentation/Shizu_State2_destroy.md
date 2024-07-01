# `Shizu_State_destroy`

*Description*
Relinquish a reference to the `Shizu_State2` singleton object.

*Signature*
```
int
Shizu_State2_destroy
  (
    Shizu_State2* SELF
  );
```
*Parameters*
- `SELF` A pointer to a `Shizu_State2` object.

*Return Value`*
A zero value on success, a non-zero value on failure.

*Details*
See [Shize_State2_destroy](Shizu_State2_destroy.md) for more information.
Multiple calls to `Shizu_State2_create` increment the reference count of that singleton object.

*Success*
The `Shizu_State2` object pointed to by `SELF` was destroyed.

*Failure*
The environment was not changed.
