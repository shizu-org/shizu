# `FileSystem.isRelativePath`

## Synopsis
Get if a path is relative.

## Signature
```
FileSystem.isRelativePath(path : String) : Boolean
```

## Return Value
`Boolean`: `True` if the path denoted by `path` is relative. `False` if that path is absolute.

## Errors
- `NumberOfArgumentsInvalid`: The number of arguments passed is not zero.
- `ArgumentTypeInvalid`: `path` is not of type `String`.
- `ArgumentValueInvalid`: `path` does not denote a valid path.
- `EnvironmentFailed`: The environment failed.
- `AllocatioFailed`: An allocation failed.

