#define SRC_BASE_STRING_STRING_UTILS_H_

#include <stdarg.h>
#include <string.h>

#include "base/compiler_special.h"

namespace base
{
template <typename Char>
inline bool IsHexDigit(Char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'F') ||
         (c >= 'a' && c <= 'f');
}

// Wrapper for vsnprintf that always null-terminates and always returns the
// number of characters that would be in an untruncated formatted
// string, even when truncation occurs.
int vsnprintf(char* buffer, size_t size, const char* format, va_list arguments)
    PRINTF_FORMAT(3, 0);

#if defined(OS_WIN)
int vswprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list arguments);
#endif
} // namespace base
