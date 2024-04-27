# `Shizu_WeakReference`

*Description*
The opaque type of a weak reference.

*Signature*
```
typedef struct <implementation> Shizu_WeakReference;
```

*Remarks*
A `Shizu_WeakReference` object is also a `Shizu_Object` object.
Consequently, whenever the address of a `Shizu_Object` object is expected, you may pass the address of a `Shizu_WeakReference`.
