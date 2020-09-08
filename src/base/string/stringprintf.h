#ifndef SRC_BASE_STRING_STRINGPRINTF_H_
#define SRC_BASE_STRING_STRINGPRINTF_H_

#include <string>

#include <stdarg.h>   // va_list

#include "base/base_export.h"

namespace base
{
// Return a C++ string given printf-like input.
BASE_EXPORT std::string StringPrintf(const char* format, ...);

// Return a C++ string given vprintf-like input.
BASE_EXPORT std::string StringPrintV(const char* format, va_list ap);

// Lower-level routine that takes a va_list and appends to a specified
// string.  All other routines are just convenience wrappers around it.
BASE_EXPORT void StringAppendV(std::string* dst, const char* format, va_list ap);
} // namespace base


#endif