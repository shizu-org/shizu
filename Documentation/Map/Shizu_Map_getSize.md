# `Shizu_Map_getSize`

*Description*
Get the size of a `Shizu_Map` value.

*Signature*
```
size_t
Shizu_Map_getSize
  (
    Shizu_State* state,
    Shizu_Map* self
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State` value.
- The behaviour of this function is undefined if `self` does not point to a `Shizu_Map` value.

*Parameters*
- `state` A pointer to a `Shizu_State` value.
- `self` A pointer to a `Shizu_Map` value.

*Return Value*
The size of the `Shizu_Map` value.

*Remarks*
The size of a `Shizu_Map` value does not exceed `INT_MAX`, `SIZE_MAX`, or `Shizu_Integer32_Maximum` whichever of the three values is the smallest.
Furthermore, the size is always non-negative.
