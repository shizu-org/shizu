# `idlib_is_power_oft_two`

## C Signature
```
bool
idlib_is_power_of_two_<suffix>
  (
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
Get if a value is a power of two (that is, if it is 1, 2, 4, ...).

## Parameters
- `x` the value of type `<type>`

## Return value
`true` if the value is a power of two. `false` otherwise.
