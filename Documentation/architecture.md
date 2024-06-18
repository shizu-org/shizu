## Introduction
*Shizu Runtime* is an interpreter.
It maintains a three-tier state represented by
a composition of three state objects.
`Shizu_State1` is the low-level tier and provides
- dynamic library handling
- allocation of memory from the operating system
- jump targets and jump
- status variable
`Shizu_State2` is the mid-level tier and provides
- precise garbage collection
- runtime type system
- includes the `Shizu_State1` tier
`Shizu_State3` is the high-level tier and provides
- stack
- (global) environment
- modules
- includes the `Shizu_State2` tier

## Environments
The runtime maintains "types" and "values".
There are two kinds of values "object values" and "scalar values".

Each "type" and each "object" has an "environment" which essentially is a mapping from a "names" (which are "object values") to a "value"s.
Environments are also "object values".

## FAQ
- **Q**: Why can't I manipulate the Shizu.State3 tier during GC visit or finalize?
- **A**: The manipulation of the Shizu.State3 tier could violate GC invariants in GC visit.
  The manipulation of Shizu.State3 tier is forbidden in finalize as it is not required so far.
