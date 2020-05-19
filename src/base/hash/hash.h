#ifndef SRC_BASE_HASH_HASH_H_
#define SRC_BASE_HASH_HASH_H_

#include <stdint.h>

#include <limits>
#include <string>

#include "base/base_export.h"

typedef std::pair<uint64_t, uint64_t> uint128_t;

namespace base {

BASE_EXPORT uint32_t Hash(const char* data, const size_t length);
BASE_EXPORT uint32_t Hash(const std::string& str);
BASE_EXPORT uint32_t Hash(const std::wstring& str);

BASE_EXPORT uint64_t Hash64(const char* data, const size_t length);
BASE_EXPORT uint64_t Hash64(const std::string& str);
BASE_EXPORT uint64_t Hash64(const std::wstring& str);

BASE_EXPORT uint128_t Hash128(const char* data, const size_t length);
BASE_EXPORT uint128_t Hash128(const std::string& str);
BASE_EXPORT uint128_t Hash128(const std::wstring& str);

}  // namespace base

#endif  // SRC_BASE_HASH_HASH_H_
