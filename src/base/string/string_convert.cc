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

}  // namespace base
