# `FileSystem.setFileContents`

## Synopsis
Set the contents of a file.

## Signature
```
FileSystem.setFileContents(path : String, contents : ByteArray) : Void
```

## Parameter Value
- `path` `String`: The path of the file.
- `contents` : `ByteArray`: The contents of the file.

## Errors
- `NumberOfArgumentsInvalid`: The number of arguments passed is not two.
- `ArgumentTypeInvalid`: `path` is not of type `String`.
- `ArgumentTypeInvalid`: `contents` is not of type `ByteArray`.
- `ArgumentValueInvalid`: `path` does not denote a valid path.
- `EnvironmentFailed`: The environment failed.
- `AllocatioFailed`: An allocation failed.
- `FileSystem.IsDirectoryError`: The file exists but is a directory.

