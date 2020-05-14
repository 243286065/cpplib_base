#ifndef BASE_SRC_BASE_THREAD_UTILS_
#define BASE_SRC_BASE_THREAD_UTILS_
 
#include <stdint.h>

#include "src/base/base_export.h"

namespace base {

uint64_t BASE_EXPORT GetCurrentThreadId();

}  // namespace base
#endif  // BASE_SRC_BASE_THREAD_UTILS_