# `Shizu_List_getSize`

*Description*
Get the size of a `Shizu_List` value.

*Signature*
```
size_t
Shizu_List_getSize
  (
    Shizu_State* state,
    Shizu_List* self
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State` value.
- The behaviour of this function is undefined if `self` does not point to a `Shizu_List` value.

*Parameters*
- `state` A pointer to a `Shizu_State` value.
- `self` A pointer to a `Shizu_List` value.

*Return Value*
The size of the `Shizu_List` value.

*Remarks*
The size of a `Shizu_List` value does not exceed `INT_MAX`, `SIZE_MAX`, or `Shizu_Integer32_Maximum` whichever of the three values is the smallest.
Furthermore, the size is always non-negative.
