#if !defined(SHIZU_RUNTIME_SOURCELOCATIONC_H_INCLUDED)
#define SHIZU_RUNTIME_SOURCELOCATIONC_H_INCLUDED

typedef struct Shizu_SourceLocationC {
  char const* file;
  int line;
} Shizu_SourceLocationC;

#define Shizu_SourceFileC() \
  __FILE__

#define Shizu_SourceLineC() \
  __LINE__

#define Shizu_SourceLocationC() \
  (Shizu_SourceLocationC) {. file = Shizu_SourceFileC(), .line = Shizu_SourceLineC() }

#endif // SHIZU_RUNTIME_SOURCELOCATIONC_H_INCLUDED
