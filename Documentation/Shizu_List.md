# `Shizu_List`

*Description*
The opaque type of a list.

*Signature*
```
typedef struct <implementation> Shizu_List;
```

*Remarks*
A `Shizu_List` object is also a `Shizu_Object` object.
Consequently, whenever the address of a `Shizu_Object` object is expected, you may pass the address of a `Shizu_List`.
