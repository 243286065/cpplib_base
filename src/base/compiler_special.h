#ifndef SRC_BASE_COMPILER_SEPICAL_H_
#define SRC_BASE_COMPILER_SEPICAL_H_

// Tell the compiler a function is using a printf-style format string.
// |format_param| is the one-based index of the format string parameter;
// |dots_param| is the one-based index of the "..." parameter.
// For v*printf functions (which take a va_list), pass 0 for dots_param.
// (This is undocumented but matches what the system C headers do.)
// For member functions, the implicit this parameter counts as index 1.
#if defined(COMPILER_GCC) || defined(__clang__)
#define PRINTF_FORMAT(format_param, dots_param) \
  __attribute__((format(printf, format_param, dots_param)))
#else
#define PRINTF_FORMAT(format_param, dots_param)
#endif

#endif  // SRC_BASE_COMPILER_SEPICAL_H_
