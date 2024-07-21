# `idlib_power_of_two_gt`

## C Signature
```
int
idlib_power_of_two_gt_<suffix>
  (
    <type>* RETURN,
    <type> x
  );
```
where `<suffix>` and `<type>` can assume the values of any of the rows from the following table

| `<suffix>` | `<type>`   |
|------------|------------|
|  `u8`      | `uint8_t`  |
| `u16`      | `uint16_t` |
| `u32`      | `uint32_t` |
| `u64`      | `uint64_t` |
|  `sz`      | `size_t`   |

## Description
Get the least power of two greater than the value `x`.

## Parameters
- `RETURN` a pointer to a `<type>` variable
- `x` the value of type `<type>`

## Return value
`IDLIB_SUCCESS` on success. One of the following return values is returned on failure:
- `IDLIB_ARGUMENT_INVALID` `RETURN` is a null pointer
- `IDLIB_NOT_REPRESENTABLE` the power of two is not representable by values of the type `<type>`

## Success
`*RETURN` was assigned the power of two.

## Failure
`*RETURN` was not dereferenced.
