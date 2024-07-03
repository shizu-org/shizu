# `FileSystem.getFileContents`

## Synopsis
Get the contents of a file.

## Signature
```
FileSystem.getFileContents(path : String) : ByteArray
```

## Parameter Values
- `path` `String`: The path of the file.

## Return Value
The contents of a file as an UTF-8 string.

## Errors
- `NumberOfArgumentsInvalid`: The number of arguments passed is not one.
- `ArgumentTypeInvalid`: `path` is not of type `String`.
- `ArgumentValueInvalid`: `path` is not a valid path.
- `EnvironmentFailed`: The environment failed.
- `AllocatioFailed`: An allocation failed.
- `FileSystem.IsDirectoryError`: The file exists but is a directory.

