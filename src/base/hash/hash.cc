#include "base/hash/hash.h"

#include "base/string/string_convert.h"
#include "base/third_party/cityhash/city.h"

namespace base {

uint32_t Hash(const char* data, const size_t length) {
  return CityHash32(data, length);
}

uint32_t Hash(const std::string& str) {
  return CityHash32(str.data(), str.size());
}

uint32_t Hash(const std::wstring& str) {
	return Hash(UnicodeToAnsi(str));
}

uint64_t Hash64(const char* data, const size_t length) {
  return CityHash64(data, length);
}

uint64_t Hash64(const std::string& str) {
  return CityHash64(str.data(), str.size());
}

uint64_t Hash64(const std::wstring& str) {
	return Hash64(UnicodeToAnsi(str));
}

uint128_t Hash128(const char* data, const size_t length) {
	return CityHash128(data, length);
}

uint128_t Hash128(const std::string& str) {
	return CityHash128(str.data(), str.size());
}

uint128_t Hash128(const std::wstring& str) {
	return Hash128(UnicodeToAnsi(str));
}

}  // namespace base
