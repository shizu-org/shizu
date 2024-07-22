# `idlib_count_leading_zeroes`

## C Signature
```
size_t
idlib_count_leading_zeroes_<suffix>
  (
    <type> x
  )
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
Get the number of leading zeroes of the binary representation of the value. the leading zeroes of an unsigned integer value.

## Parameters
- `x` The value.

## Return value
The number of leading zeroes of the binary representation of the value.
