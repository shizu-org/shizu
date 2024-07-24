# n Bit alignment
What does n Bit alignment mean where n > 0.

The address of a byte is a non-negative decimal value.
That decimal value is divisible without rest by n.

The GC stores the alignment in DefaultGc.alignment. That value is usually 32 or 64 bit.
The alignment is usually determined the void pointer size.

# Starting up and shutting down
The GC is started up by a call to
```
Shizu_Gc_Status
Shizu_Gc_startup
  (
  );
```
A zero value is returned on success. `Shizu_Gcx_ReferenceCountOverflow`, `Shizu_Gcx_LockFailed` and `Shizu_Gcx_AllocationFailed` are returned on failure.
Everytime this function is successfully invoked, a reference count is incremented.

```
Shizu_Gc_Statust
Shizu_Gc_shutdown
  (
  );
```
A zero value is returned on success. A non-zero value is returned on failure.
Everytime this function is successfully invoked, a reference count is incremented. 

# The tag
The tag is of size `gc.alignment * n, n > 0`. For the current implementation, `n` is `2`.
The tag address is aligned to `gc.alignment`.
Consequenlty, the payload address is also aligned to `gc.alignment`.

# The types.
A GC type consists of a function
```
void (Shizu_Gc_VisitCallback)(void* visitContext, void* object);
void (Shizu_Gc_FinalizeCallback)(void* finalizeContext, void* object);
```

GC types are registered with the GC using
```
Shizu_Gc_Status
Shizu_Gc_registerType
  (
    char const* name,
    size_t nameLength,
    void* visitContext,
    Shizu_Gcx_VisitCallback* visitCallback,
    void* finalizeContext,
    Shizu_Gcx_FinalizeCallback* finalizeCallback
  );
```
`Shizu_Gc_Status_Success` is returned on success.
`Shizu_Gc_Status_TypeExists` is returned if a type of the same name already exists.
`Shizu_Gc_Status_ArgumentInvalid` is returned if `name`.
The initial reference count of a registered type is `1`.

The visit context is an opaque pointer passed to the visit callback as its first argument.
The visit callback can be null.

The finalize context is an opaque pointer passed to the finalize callback as its first argument.
The finalize callback can be null.

You can unregister a GC type by calling
```
Shizu_Gc_Result
Shizu_Gc_unregisterType
  (
    char const* name,
    size_t nameLength
  );
```
`Shizu_Gc_Status_Success` is returned on success.
`Shizu_Gc_Status_TypeNotExists` is returned if a type of the name does not exist.
`Shizu_Gc_Status_ArgumentInvalid` is returned if `name` is null.
`Shizu_Gc_Status_ReferenceCounterOverflow` is returned if the reference count is zero and the would have been decremented further by this call.

However, the type is not unregistered until no more objects of that type exist.
If the type is in unsed, then `Shizu_Gc_Status_TypeUsed` is returned.
This decrements the reference count of the type by `1`.

## Acquire and relinquish types
Acquire increments the reference count of a type by `1`.
Relinquish decrements the reference count of a type by `1`.

## Allocation
```
Shizu_Gcx_Status
Shizu_Gcx_allocate
  (
    size_t size,
    Shizu_Gcx_Type* type
  )
```
`Shizu_Gc_Status_ArgumentInvalid` is returned if `type` is null.
`Shizu_Gc_Status_ReferenceCounterOverflow` is returned if the reference count is at its maximum and the would have been incremented further by this call.
A successful allocation increments the reference count of `type` by `1`.

The memory `sizeof(Tag)+size` is allocated, a pointer to the Byte after the tag is returned in `*object` on success.
The tag stores a pointer to the type. The tag is stored in a singly-linked list of the type. (Consequently, a tag is of size 2 * gc.alignment 
`object` is not dereferenced on failure.

# Run
Types are registered at nodes. During visitation, objects of a type re-use their tag to queue on their nodes.