# `idlib_status`

## C Signature
```
typedef <implementation> idlib_status;
```

## Description
Type of a non-negative integer values indicating the outcome of an operation.

The values of this type are denoted by symbolic constants.

The following table lists the name of the symbolic constant in its left column and the description of the symbolic constant in its right column.

| Symbolic constant          | Description                                                                                                                       |
|----------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| `IDLIB_SUCCESS`            | Idicates success. This constant is the only status constant guaranteed to evaluate to the zero integer value.                     |
| `IDLIB_ENVIRONMENT_FAILED` | Indicates failure because the environment failed.                                                                                 |
| `IDLIB_ARGUMENT_INVALID`   | Indicates failure because of an invalid argument.                                                                                 |
| `IDLIB_ABORTED`            | Indicates failure because something was aborted.                                                                                  |
| `IDLIB_LOCKED`             | Indicates failure because something was locked.                                                                                   |
| `IDLIB_UNLOCKED`           | Indicates failure because something was unlocked.                                                                                 |
| `IDLIB_LOCK_FAILED`        | Indicates failure because locking failed.                                                                                         |
| `IDLIB_ALLOCATION_FAILED`  | Indicates failure because an allocation failed.                                                                                   |
| `IDLIB_TOO_BIG`            | Indicates failure because something was too big.                                                                                  |
| `IDLIB_TOO_SMALL`          | Indicates failure because something was too small.                                                                                |
| `IDLIB_OVERFLOW`           | Indicates failure because of an overflow.                                                                                         |
| `IDLIB_UNDERFLOW`          | Indicates failure because of an underflow.                                                                                        |
| `IDLIB_UNDERFLOW`          | Indicates failure because of an underflow.                                                                                        |
| `IDLIB_EXISTS`             | Indicates failure because something existed (but should not have).                                                                |
| `IDLIB_NOT_EXISTS`         | Indicates failure because something did not exist (but should have existed).                                                      |
| `IDLIB_NOT_REPRESENTABLE`  | Indicates failure because a value is not representable by a type.                                                                 |
| `IDLIB_ALREADY_STARTED`    | Indicates failure because something was already started.                                                                          |
| `IDLIB_ALREADY_STOPPED`    | Indicates failure because something was already stopped.                                                                          |
