#include "base/string/string_utils.h"

#include <ctype.h>
#include <stdarg.h>   // va_list
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

namespace base {

int vsnprintf(char* buffer,
              size_t size,
              const char* format,
              va_list arguments) {
  return ::vsnprintf(buffer, size, format, arguments);
}

}  // namespace base
