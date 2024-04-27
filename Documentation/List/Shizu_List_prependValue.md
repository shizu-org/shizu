# `Shizu_List_prependValue`

*Description*
Prepend a `Shizu_Value` value to a `Shizu_List` value.
If the `Shizu_Value` value stores a `Shizu_Void` value, nothing is prepended.

*Signature*
```
void
Shizu_List_prependValue
  (
    Shizu_State* state,
    Shizu_List* self,
    Shizu_Value const* value
  );
```

*Remarks*
- The behaviour of this function is undefined if `state` does not point to a `Shizu_State` value.
- The behaviour of this function is undefined if `self` does not point to a `Shizu_List` value.
- The behaviour of this function is undefined if `value` does not point to a `Shizu_Value` value.

*Parameters*
- `state` A pointer to a `Shizu_State` value.
- `self` A pointer to a `Shizu_List` value.
- `value` A pointer to a `Shizu_Value` value.
