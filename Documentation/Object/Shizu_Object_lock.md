# `Shizu_Object_lock`

*Description*
Add a lock to a `Shizu_Object` object.

*Signature*
```
void
Shizu_Object_lock
  (
    Shizu_State* state,
    Shizu_Object* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Object` object.
This function raises an error if the maximum number of locks per object would overflow.
This maximum number is usually `Shizu_Integer32_Maximum`.

*Parameters*
- `self` A pointer to a `Shizu_Object` object.
