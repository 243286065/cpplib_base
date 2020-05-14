#ifndef SRC_BASE_THREAD_UTILS_H_
#define SRC_BASE_THREAD_UTILS_H_

#include <stdint.h>

#include "src/base/base_export.h"

namespace base {

uint64_t BASE_EXPORT GetCurrentThreadId();

}  // namespace base
#endif  // SRC_BASE_THREAD_UTILS_H_
