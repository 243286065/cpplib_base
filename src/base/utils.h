#ifndef SRC_BASE_UTILS_H_
#define SRC_BASE_UTILS_H_

#include <stdint.h>

#include <string>

#include "base/base_export.h"

namespace base {

// Get current thread id
BASE_EXPORT uint64_t GetTaskCurrentThreadId();

// Get current process id
BASE_EXPORT uint32_t GetTaskCurrentProcessId();

BASE_EXPORT std::string GetFileBaseName(const std::string& path);

BASE_EXPORT int64_t GetMillSecondsTimestamp();


}  // namespace base
#endif  // SRC_BASE_UTILS_H_
