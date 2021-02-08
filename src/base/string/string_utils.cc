#include "base/string/string_utils.h"
#include "base/string/string_utils_internal.h"

#include <ctype.h>
#include <stdarg.h>   // va_list
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace base {

int vsnprintf(char* buffer,
              size_t size,
              const char* format,
              va_list arguments) {
  return ::vsnprintf(buffer, size, format, arguments);
}

#if defined(OS_WIN)
int vswprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list arguments) {
  return ::vswprintf(buffer, size, format, arguments);
}
#endif

std::string ToLowerASCII(const std::string& str) {
  return internal::ToLowerASCIIImpl(str);
}

std::wstring ToLowerASCII(const std::wstring& str) {
  return internal::ToLowerASCIIImpl(str);
}

// Converts the given string to it's ASCII-uppercase equivalent.
std::string ToUpperASCII(const std::string& str) {
  return internal::ToUpperASCIIImpl(str);
}

std::wstring ToUpperASCII(const std::wstring& str) {
  return internal::ToUpperASCIIImpl(str);
}

int CompareCaseInsensitiveASCII(const std::string& a, const std::string& b) {
  return internal::CompareCaseInsensitiveASCIIT(a, b);
}

int CompareCaseInsensitiveASCII(const std::wstring& a, const std::wstring& b) {
  return internal::CompareCaseInsensitiveASCIIT(a, b);
}

bool EqualsCaseInsensitiveASCII(const std::string& a, const std::string& b) {
  return a.size() == b.size() &&
         internal::CompareCaseInsensitiveASCIIT(a, b) == 0;
}

bool EqualsCaseInsensitiveASCII(const std::wstring& a, const std::wstring& b) {
  return a.size() == b.size() &&
         internal::CompareCaseInsensitiveASCIIT(a, b) == 0;
}

// ----------------------------StartsWith/EndsWith-----------------------

bool StartsWith(
    const std::string& str,
    const std::string& search_for,
    CompareCase case_sensitivity) {
  return internal::StartsWithT(str, search_for, case_sensitivity);
}

bool StartsWith(
    const std::wstring& str,
    const std::wstring& search_for,
    CompareCase case_sensitivity) {
  return internal::StartsWithT(str, search_for, case_sensitivity);
}

bool EndsWith(
    const std::string& str,
    const std::string& search_for,
    CompareCase case_sensitivity) {
  return internal::EndsWithT(str, search_for, case_sensitivity);
}

bool EndsWith(
    const std::wstring& str,
    const std::wstring& search_for,
    CompareCase case_sensitivity) {
  return internal::EndsWithT(str, search_for, case_sensitivity);
}

bool IsDigitStrs(const std::string& str) {
  for (int i = 0; i < str.size(); i++) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool IsDigitStrs(const std::wstring& str) {
  for (int i = 0; i < str.size(); i++) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> SplitStr(const std::string& s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

bool LowerCaseEqualsASCII(const std::string& str,
                          const std::string& lowercase_ascii) {
  return internal::DoLowerCaseEqualsASCII(str, lowercase_ascii);
}
bool LowerCaseEqualsASCII(const std::wstring& str,
                          const std::wstring& lowercase_ascii) {
  return internal::DoLowerCaseEqualsASCII(str, lowercase_ascii);
}

}  // namespace base
