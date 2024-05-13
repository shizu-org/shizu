#if !defined(FILESYSTEM_H_INCLUDED)
#define FILESYSTEM_H_INCLUDED

#include "Shizu/Runtime/Include.h"

Shizu_declareType(Shizu_FileSystem);

struct Shizu_FileSystem {
  Shizu_Object parent;
};

struct Shizu_FileSystem_Dispatch {
  Shizu_Object_Dispatch parent;
};

/**
 * @since 1.0
 * @brief Create a Shizu_FileSystem object.
 * @param state A pointer to a Shizu_State object.
 * @return A pointer to the Shizu_FileSystem object.
 */
Shizu_FileSystem*
Shizu_FileSystem_create
  (
    Shizu_State* state
  );

#endif // FILESYSTEM_H_INCLUDED
