# `Shizu_State_create`

*Description*
Acquire a reference to the `Shizu_State` singleton object.

*Signature*
```
int
Shizu_State_create
  (
    Shizu_State** RETURN
  );
```

*Details*
Each call to `Shizu_State_create` acquires a reference to that singleton object.
If no longer required, the reference must be relinquished using `Shizu_State_destroy`.
If there are no more references, the singleton object is destroyed.

*Parameters*
- `RETURN` A pointer to a `Shizu_State` object.

*Success*
`*RETURN` was assigned a pointer to the `Shizu_State` singleton object.
The caller acquired a reference to that object.

*Failure*
`RETURN` was not dereferenced.
The caller did not acquire a reference to `Shizu_State` singleton object. 
