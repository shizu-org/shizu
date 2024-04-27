# `Shizu_Object_unlock`

*Description*
Remove a lock from a `Shizu_Object` object.

*Signature*
```
void
Shizu_Object_unlock
  (
    Shizu_State* state,
    Shizu_Object* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Object` object.
The behaviour of this function is undefined if the `Shizu_Object` object has no locks.

*Parameters*
- `self` A pointer to a `Shizu_Object` object.
