
#if !defined(SHIZU_RUNTIME_JUMPTARGET_H_INCLUDED)
#define SHIZU_RUNTIME_JUMPTARGET_H_INCLUDED

#include "Shizu/Runtime/Configure.h"

#define _GNU_SOURCE

#include <setjmp.h>

typedef struct Shizu_JumpTarget Shizu_JumpTarget;

struct Shizu_JumpTarget {
  Shizu_JumpTarget* previous;
  jmp_buf environment;
};

#endif // SHIZU_RUNTIME_JUMPTARGET_H_INCLUDED
