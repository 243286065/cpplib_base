#define SRC_BASE_STRING_STRING_UTILS_H_

#include <stdarg.h>
#include <string.h>
#include <string>
#include <type_traits>
#include <vector>

#include "base/base_export.h"
#include "base/compiler_special.h"

namespace base
{
template <typename Char>
inline bool IsHexDigit(Char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'F') ||
         (c >= 'a' && c <= 'f');
}

// Wrapper for vsnprintf that always null-terminates and always returns the
// number of characters that would be in an untruncated formatted
// string, even when truncation occurs.
int vsnprintf(char* buffer, size_t size, const char* format, va_list arguments)
    PRINTF_FORMAT(3, 0);

#if defined(OS_WIN)
int vswprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list arguments);
#endif

// ASCII-specific tolower.  The standard library's tolower is locale sensitive,
// so we don't want to use it here.
template <typename CharT,
          typename = enable_if_t<std::is_integral<CharT>::value>>
CharT ToLowerASCII(CharT c) {
  return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

// ASCII-specific toupper.  The standard library's toupper is locale sensitive,
// so we don't want to use it here.
template <typename CharT,
          typename = enable_if_t<std::is_integral<CharT>::value>>
CharT ToUpperASCII(CharT c) {
  return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
}

// Converts the given string to it's ASCII-lowercase equivalent.
BASE_EXPORT std::string ToLowerASCII(const std::string& str);
BASE_EXPORT std::wstring ToLowerASCII(const std::wstring& str);

// Converts the given string to it's ASCII-uppercase equivalent.
BASE_EXPORT std::string ToUpperASCII(const std::string& str);
BASE_EXPORT std::wstring ToUpperASCII(const std::wstring& str);

template<typename Char> struct CaseInsensitiveCompareASCII {
 public:
  bool operator()(Char x, Char y) const {
    return ToLowerASCII(x) == ToLowerASCII(y);
  }
};

// Like strcasecmp for case-insensitive ASCII characters only. Returns:
//   -1  (a < b)
//    0  (a == b)
//    1  (a > b)
// (unlike strcasecmp which can return values greater or less than 1/-1). For
// full Unicode support, use base::i18n::ToLower or base::i18h::FoldCase
// and then just call the normal string operators on the result.
BASE_EXPORT int CompareCaseInsensitiveASCII(const std::string& a, const std::string& b);
BASE_EXPORT int CompareCaseInsensitiveASCII(const std::wstring& a, const std::wstring& b);

// Equality for ASCII case-insensitive comparisons. For full Unicode support,
// use base::i18n::ToLower or base::i18h::FoldCase and then compare with either
// == or !=.
BASE_EXPORT bool EqualsCaseInsensitiveASCII(const std::string& a, const std::string& b);
BASE_EXPORT bool EqualsCaseInsensitiveASCII(const std::wstring& a, const std::wstring& b);

// Indicates case sensitivity of comparisons. Only ASCII case insensitivity
// is supported. Full Unicode case-insensitive conversions would need to go in
// base/i18n so it can use ICU.
//
// If you need to do Unicode-aware case-insensitive StartsWith/EndsWith, it's
// best to call base::i18n::ToLower() or base::i18n::FoldCase() (see
// base/i18n/case_conversion.h for usage advice) on the arguments, and then use
// the results to a case-sensitive comparison.
enum class CompareCase {
  SENSITIVE,
  INSENSITIVE_ASCII,
};

BASE_EXPORT bool StartsWith(
    const std::string& str,
    const std::string& search_for,
    CompareCase case_sensitivity = CompareCase::SENSITIVE);
BASE_EXPORT bool StartsWith(
    const std::wstring& str,
    const std::wstring& search_for,
    CompareCase case_sensitivity = CompareCase::SENSITIVE);
BASE_EXPORT bool EndsWith(
    const std::string& str,
    const std::string& search_for,
    CompareCase case_sensitivity = CompareCase::SENSITIVE);
BASE_EXPORT bool EndsWith(
    const std::wstring& str,
    const std::wstring& search_for,
    CompareCase case_sensitivity = CompareCase::SENSITIVE);

BASE_EXPORT bool IsDigitStrs(const std::string& str);
BASE_EXPORT bool IsDigitStrs(const std::wstring& str);

BASE_EXPORT std::vector<std::string> SplitStr(const std::string& s, char delim);

// Compare the lower-case form of the given string against the given
// previously-lower-cased ASCII string (typically a constant).
BASE_EXPORT bool LowerCaseEqualsASCII(const std::string& str,
                                      const std::string& lowercase_ascii);
BASE_EXPORT bool LowerCaseEqualsASCII(const std::wstring& str,
                                      const std::string& lowercase_ascii);
} // namespace base
