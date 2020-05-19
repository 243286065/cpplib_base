#include "base/string/string_convert.h"

#include <locale.h>
#include <string.h>

#include <algorithm>
#include <sstream>
#include <vector>

namespace base {

void AnsiToUnicode(wchar_t* dest, const char* data, const size_t length) {
  std::string curr_local = setlocale(LC_ALL, NULL);
#if (defined WIN32) || (defined _WIN32)
  setlocale(LC_ALL, "chs");
#else
  setlocale(LC_ALL, "");
#endif

  size_t size = length + 1;
  wmemset(dest, 0, size);
  mbstowcs(dest, data, size);

  setlocale(LC_ALL, curr_local.c_str());
}

void UnicodeToAnsi(char* dest, const wchar_t* data, const size_t length) {
  std::string curr_local = setlocale(LC_ALL, NULL);
#if (defined WIN32) || (defined _WIN32)
  setlocale(LC_ALL, "chs");
#else
  setlocale(LC_ALL, "");
#endif

  size_t size = 2 * length + 1;
  memset(dest, 0, size);
  wcstombs(dest, data, size);
  setlocale(LC_ALL, curr_local.c_str());
}

std::wstring AnsiToUnicode(const std::string& data) {
  size_t length = data.size();
  wchar_t* dest = new wchar_t[length + 1];
  AnsiToUnicode(dest, data.c_str(), length);
  std::wstring result(dest);
  delete[] dest;
  return result;
}

std::string UnicodeToAnsi(const std::wstring& data) {
  size_t length = data.size();
  char* dest = new char[2 * length + 1];
  UnicodeToAnsi(dest, data.c_str(), length);
  std::string result(dest);
  delete[] dest;
  return result;
}

std::string StrToUpper(const std::string& str) {
  std::string newStr = str;
  std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::toupper);
  return newStr;
}

bool IsDigitStrs(const std::string& str) {
  for (int i = 0; i < str.size(); i++) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool StartWith(const std::string& target, const std::string& sub) {
  return target.find(sub) == 0 ? true : false;
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

}  // namespace base
