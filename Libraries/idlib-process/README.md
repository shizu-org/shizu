# IdLib Process
*IdLib Process* is a supporting library of *Shizu*.
It provides cross-platform (Windows and Linux) intra-process functionality for embedding into C programs:
- singleton root object that is thread-safe, reference counted, and retains its singleton property also when using DLLs in Windows.
- singleton-scoped recursive intra-process mutex.
*IdLib Process* is always licensed under the same license as *Shizu* (which is the [zlib License](./../../LICENSE)).

## Integrating into Shizu
Integrating *IdLib Process* into *Shizu* is the only intended use of *IdLib Process*.
For information on the integration, see [Integrating *IdLib Process* into *Shizu*](integrating-into-shizu.md).

## Documentation
The documentation is provided as a set of MarkDown files directly in this repository.

Start reading the documentation here [documentation/idlib-process.md](documentation/idlib-process.md).
