# idlib_existing_file_policy

## C Signature
````
typedef enum <implementation { <elements> } idlib_existing_file_policy;
```

## Decription
An enumeration of policies for opening a file that exists.
The enumeration elements cannot be OR'ed together.

## Elements
- `idlib_non_existing_file_policy_fail` Fail if the file does not exist.
- `idlib_non_existing_file_policy_create` Create the file if the file does not exist.
