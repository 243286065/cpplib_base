#ifndef SRC_BASE_FILE_FILE_UTILS_H_
#define SRC_BASE_FILE_FILE_UTILS_H_

#include "base/base_export.h"

namespace base {
BASE_EXPORT void DeleteFilePath(const char* file_path);

BASE_EXPORT bool WriteFile(const char* file_path, const char* message, bool append);
}  // namespace base

#endif  // SRC_BASE_FILE_FILE_UTILS_H_
