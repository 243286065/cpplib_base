#ifndef SRC_BASE_HASH_MD5_H_
#define SRC_BASE_HASH_MD5_H_

#include <string>

#include "base/base_export.h"

namespace base {

std::string FastMD5(const std::string& input);

}  // namespace base

#endif  // SRC_BASE_HASH_MD5_H_