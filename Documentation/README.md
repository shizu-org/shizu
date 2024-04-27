# Shizu Runtime
*Shizu Runtime* is the underlaying execution environment in which the server and its clients are implemented.

## C Interface Documentation

### Conventions
The C code of *Shizu Runtime* follows conventions in the source code as well as the documentation of the source code.

A `Shizu_Reference(T)` object is a pointer to a `T` object where `T` is one of the following:
- `Shizu_CxxFunction` type
- `Shizu_Object` type (or derived type).
In particular, these pointers may *not* be null.

`Shizu_MaybeReference(T)` object is a pointer to an `T` object or the null pointer where `T` is one of the following:
- `Shizu_CxxFunction` type
- `Shizu_Object` type (or derived type).
These pointers may be null.

### Documentation
It follows the documentation of the *Shizu Runtime* C Interface in alphabetic oder.

*A*

*B*
- [Shizu_Boolean](Shizu_Boolean.md)
- [Shizu_Boolean_True](Shizu_Boolean_True.md)
- [Shizu_Boolean_False](Shizu_Boolean_False.md)

*C*

*F*
- [Shizu_Float32](Shizu_Float32.md)
- [Shizu_Float32_Maximum](Shizu_Float32_Maximum.md)
- [Shizu_Float32_Minimum](Shizu_Float32_Minimum.md)

*G*

*H*

*I*
- [Shizu_Integer32](Shizu_Integer32.md)
- [Shizu_Integer32_Maximum](Shizu_Integer32_Maximum.md)
- [Shizu_Integer32_Minimum](Shizu_Integer32_Minimum.md)

*J*
- [Shizu_JumpTarget](Shizu_JumpTarget.md)

*K*

*L*
- [Shizu_List](Shizu_List.md)
- [Shizu_List_appendValue](Shizu_List_appendValue.md)
- [Shizu_List_getSize](Shizu_List_getSize.md)
- [Shizu_List_insertValue](Shizu_List_insertValue.md)
- [Shizu_List_prependValue](Shizu_List_prependValue.md)

*M*
- [Shizu_Map](Shizu_Map.md)

*N*

*O*
- [Shizu_Object](Shizu_Object.md)
- [Shizu_Object_isBlack](Object/Shizu_Object_isBlack.md)
- [Shizu_Object_isGray](Object/Shizu_Object_isGray.md)
- [Shizu_Object_isWhite](Object/Shizu_Object_isWhite.md)
- [Shizu_Object_lock](Object/Shizu_Object_lock.md)
- [Shizu_Object_setBlack](Object/Shizu_Object_setBlack.md)
- [Shizu_Object_setGray](Object/Shizu_Object_setGray.md)
- [Shizu_Object_setWhite](Object/Shizu_Object_setWhite.md)
- [Shizu_Object_unlock](Object/Shizu_Object_unlock.md)

*P*

*Q*

*R*

*S*
- [Shizu_Stack_peek](Stack/Shizu_Stack_getSize.md)
- [Shizu_Stack_peek](Stack/Shizu_Stack_peek.md)
- [Shizu_Stack_pop](Stack/Shizu_Stack_pop.md)
- [Shizu_Stack_push](Stack/Shizu_Stack_push.md)
- [Shizu_Stack_getBoolean](Stack/Shizu_Stack_getBoolean.md)
- [Shizu_Stack_getCxxFunction](Stack/Shizu_Stack_getCxxFunction.md)
- [Shizu_Stack_getFloat32](Stack/Shizu_Stack_getFloat32.md)
- [Shizu_Stack_getInteger32](Stack/Shizu_Stack_getInteger32.md)
- [Shizu_Stack_getObject](Stack/Shizu_Stack_getObject.md)
- [Shizu_Stack_getVoid](Stack/Shizu_Stack_getVoid.md)
- [Shizu_Stack_isBoolean](Stack/Shizu_Stack_isBoolean.md)
- [Shizu_Stack_isCxxFunction](Stack/Shizu_Stack_isCxxFunction.md)
- [Shizu_Stack_isFloat32](Stack/Shizu_Stack_isFloat32.md)
- [Shizu_Stack_isInteger32](Stack/Shizu_Stack_isInteger32.md)
- [Shizu_Stack_isObject](Stack/Shizu_Stack_isObject.md)
- [Shizu_Stack_isVoid](Stack/Shizu_Stack_isVoid.md)
- [Shizu_Stack_pushBoolean](Stack/Shizu_Stack_pushBoolean.md)
- [Shizu_Stack_pushCxxFunction](Stack/Shizu_Stack_pushCxxFunction.md)
- [Shizu_Stack_pushFloat32](Stack/Shizu_Stack_pushFloat32.md)
- [Shizu_Stack_pushInteger32](Stack/Shizu_Stack_pushInteger32.md)
- [Shizu_Stack_pushObject](Stack/Shizu_Stack_pushObject.md)
- [Shizu_Stack_pushVoid](Stack/Shizu_Stack_pushVoid.md)
- [Shizu_State](Shizu_State.md)
- [Shizu_State_create](Shizu_State_create.md)
- [Shizu_State_destroy](Shizu_State_destroy.md)
- [Shizu_State_popJumpTarget](Shizu_State_popJumpTarget.md)
- [Shizu_State_pushJumpTarget](Shizu_State_pushJumpTarget.md)
- [Shizu_String](Shizu_String.md)

*T*

*U*

*V*

- [Shizu_Value](Shizu_Value.md)
- [Shizu_Value_getBoolean](Shizu_Value_getBoolean.md)
- [Shizu_Value_getFloat32](Shizu_Value_getFloat32.md)
- [Shizu_Value_getInteger32](Shizu_Value_getInteger32.md)
- ~~[Shizu_Value_getList](Obsolete/Shizu_Value_getList.md)~~
- ~~[Shizu_Value_getMap](Obsolete/Shizu_Value_getMap.md)~~
- ~~[Shizu_Value_getString](Obsolete/Shizu_Value_getString.md)~~
- [Shizu_Value_getVoid](Shizu_Value_getVoid.md)
- ~~[Shizu_Value_getWeakReference](Obsolete/Shizu_Value_getWeakReference.md)~~

- [Shizu_Value_isBoolean](Shizu_Value_isBoolean.md)
- [Shizu_Value_isFloat32](Shizu_Value_isFloat32.md)
- [Shizu_Value_isInteger32](Shizu_Value_isInteger32.md)
- ~~[Shizu_Value_isList](Obsolete/Shizu_Value_isList.md)~~
- ~~[Shizu_Value_isMap](Obsolete/Shizu_Value_isMap.md)~~
- ~~[Shizu_Value_isString](Obsolete/Shizu_Value_isString.md)~~
- [Shizu_Value_isVoid](Shizu_Value_isVoid.md)
- ~~[Shizu_Value_isWeakReference](Obsolete/Shizu_Value_isWeakReference.md)~~

- [Shizu_Value_setBoolean](Shizu_Value_setBoolean.md)
- [Shizu_Value_setFloat32](Shizu_Value_setFloat32.md)
- [Shizu_Value_setInteger32](Shizu_Value_setInteger32.md)
- ~~[Shizu_Value_setList](Obsolete/Shizu_Value_setList.md)~~
- ~~[Shizu_Value_setMap](Obsolete/Shizu_Value_setMap.md)~~
- ~~[Shizu_Value_setString](Obsolete/Shizu_Value_setString.md)~~
- [Shizu_Value_setVoid](Shizu_Value_setObject.md)
- [Shizu_Value_setVoid](Shizu_Value_setVoid.md)
- ~~[Shizu_Value_setWeakReference](Shizu_Value_setWeakReference.md)~~

- [Shizu_Void](Shizu_Void.md)
- [Shizu_Void_Void](Shizu_Void_Void.md)

*W*
- [Shizu_WeakReference](Shizu_WeakReference.md)

## Additional References

- [1] https://en.cppreference.com/w/c/language
