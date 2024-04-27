# `Shizu_Object_getWhite`

*Description*
Get if a `Shizu_Object` object is colored white.

*Signature*
```
bool
Shizu_Object_getWhite
  (
    Shizu_Object* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Object` object.

*Parameters*
- `self` A pointer to a `Shizu_Object` object.

*Return Value*`
`true` if the `Shizu_Object` object is colored white. Otherwise `false`.
