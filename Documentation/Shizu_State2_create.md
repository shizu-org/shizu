# `Shizu_State2_create`

*Description*
Acquire a reference to the `Shizu_State2` singleton object.

*Signature*
```
int
Shizu_State2_create
  (
    Shizu_State2** RETURN
  );
```

*Details*
Each call to `Shizu_State2_create` acquires a reference to that singleton object.
If no longer required, the reference must be relinquished using `Shizu_State2_destroy`.
If there are no more references, the singleton object is destroyed.

*Parameters*
- `RETURN` A pointer to a `Shizu_State2` object.

*Success*
`*RETURN` was assigned a pointer to the `Shizu_State2` singleton object.
The caller acquired a reference to that object.

*Failure*
`RETURN` was not dereferenced.
The caller did not acquire a reference to `Shizu_State2` singleton object. 
