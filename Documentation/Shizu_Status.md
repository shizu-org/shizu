# `Shizu_getStatus`

*Description*
Enumeration of status values.

*Signature*
```
typedef enum Shizu_Status { <implementation> } Shizu_Status
```

*Elements*
- `Shizu_Status_NoError` The initial value of the status variable of a `Shizu_State` object is `Shizu_Status_NoError`.
  This value is the only enumeration element that evaluates to zero.
- `Shizu_Status_AllocationFailed` Status value indicating that an allocation failed.
- `Shizu_Status_InvalidArgument` Status value indicating an invallid argument.
- `Shizu_Status_InvalidOperation` Status value indicating an invalid operation.
- `Shizu_Status_UnreachabelCodeReached` Status code indicating unreachable code was reached.
- `Shizu_Status_StackOverflow` Status code indicating a stack overflow.
  A stack overflow can be raised by fixed stack size interpreters and operations that expect a free capacity.
- `Shizu_Status_StackUnderflow` Status code indicating a stack underflow.
  A stack underflow occurs if more elements are popped than that are available.
- `Shizu_Status_StackIndexOutOfBounds` Status code indicating a stack index was out of bounds.
- `Shizu_Status_StackCorruption` Status code indicating a stack corruption.
- `Shizu_Status_LexicalError` Status code indicating a lexical error.
- `Shizu_Status_SyntacticalError` Status code indicating a syntactical error.
- `Shizu_Status_SemanticalError` Status code indicating a semantical error.
- `Shizu_Status_EnvironmentFailed` Status code indicating the environment failed.
- `Shizu_Status_ConversionFailed` Status code indicating a conversion failed.
