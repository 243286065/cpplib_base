#ifndef SRC_BASE_STRING_STRING_UTILS_INTERNAL_H__
#define SRC_BASE_STRING_STRING_UTILS_INTERNAL_H__



#include <string>

#include "base/log/logging.h"

namespace base
{
namespace internal
{

template <typename StringType>
StringType ToLowerASCIIImpl(const StringType& str) {
  StringType ret;
  ret.reserve(str.size());
  for (size_t i = 0; i < str.size(); i++)
    ret.push_back(ToLowerASCII(str[i]));
  return ret;
}

template <typename StringType>
StringType ToUpperASCIIImpl(const StringType& str) {
  StringType ret;
  ret.reserve(str.size());
  for (size_t i = 0; i < str.size(); i++)
    ret.push_back(ToUpperASCII(str[i]));
  return ret;
}

template <class StringType>
int CompareCaseInsensitiveASCIIT(const StringType& a,
                                 const StringType& b) {
  // Find the first characters that aren't equal and compare them.  If the end
  // of one of the strings is found before a nonequal character, the lengths
  // of the strings are compared.
  size_t i = 0;
  while (i < a.length() && i < b.length()) {
    typename StringType::value_type lower_a = ToLowerASCII(a[i]);
    typename StringType::value_type lower_b = ToLowerASCII(b[i]);
    if (lower_a < lower_b)
      return -1;
    if (lower_a > lower_b)
      return 1;
    i++;
  }

  // End of one string hit before finding a different character. Expect the
  // common case to be "strings equal" at this point so check that first.
  if (a.length() == b.length())
    return 0;

  if (a.length() < b.length())
    return -1;
  return 1;
}

template <typename Str>
bool StartsWithT(const Str& str,
                 const Str& search_for,
                 CompareCase case_sensitivity) {
  if (search_for.size() > str.size())
    return false;

  Str source = str.substr(0, search_for.size());
  // LOG(INFO) << source << ":" << search_for;

  switch (case_sensitivity) {
    case CompareCase::SENSITIVE:
      return source == search_for;

    case CompareCase::INSENSITIVE_ASCII:
      return std::equal(
          search_for.begin(), search_for.end(), source.begin(),
          CaseInsensitiveCompareASCII<typename Str::value_type>());

    default:
      NOTREACHED();
      return false;
  }
}

template <typename Str>
bool EndsWithT(const Str& str,
               const Str& search_for,
               CompareCase case_sensitivity) {
  if (search_for.size() > str.size())
    return false;

  Str source = str.substr(str.size() - search_for.size(), search_for.size());

  switch (case_sensitivity) {
    case CompareCase::SENSITIVE:
      return source == search_for;

    case CompareCase::INSENSITIVE_ASCII:
      return std::equal(
          source.begin(), source.end(), search_for.begin(),
          CaseInsensitiveCompareASCII<typename Str::value_type>());

    default:
      NOTREACHED();
      return false;
  }
}

} // namespace internal
} // namespace base

#endif // SRC_BASE_STRING_STRING_UTILS_INTERNAL_H__