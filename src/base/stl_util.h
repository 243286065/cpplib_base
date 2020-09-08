#ifndef SRC_BASE_STL_UTIL_H_
#define SRC_BASE_STL_UTIL_H_

#include <type_traits>

#include <string.h>

namespace base
{

template <typename T, size_t N>
constexpr size_t size(const T (&array)[N]) noexcept {
  return N;
}

} // namespace base

#endif