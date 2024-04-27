# `Shizu_List_getValue`

*Description*
Get a `Shizu_List` value in a `Shizu_List` value.

*Signature*
```
Shizu_Value
Shizu_List_getValue
  (
    Shizu_State* state,
    Shizu_List* self,
    size_t index
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State` value.
- The behaviour of this function is undefined if `self` does not point to a `Shizu_List` value.

*Parameters*
- `state` A pointer to a `Shizu_State` value.
- `self` A pointer to a `Shizu_List` value.
- `index` A `size_t` value. The index at which to get the `Shizu_Value` value in the `Shizu_List` value from.

*Return Value*
The `Shizu_List` value.

*Errors*
- `index` is greater than or the size of the `Shizu_List` value.
