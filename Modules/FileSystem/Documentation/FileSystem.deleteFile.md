# `FileSystem.deleteFile`

## Synopsis
Delete a file.

## Signature
```
FileSystem.deleteFile(path : String) : Void
```

## Parameter Value
- `path` `String`: The path of the file.

## Errors
- `ArgumentInvalid`: `path` is not of type `String`
- `ArgumentInvalid`: `path` is of type `String` but not a valid path
- `EnvironmentFailed`: The environment failed.
- `AllocatioFailed`: An allocation failed.

