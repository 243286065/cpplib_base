#include "base/utils.h"

#include <algorithm>
#include <chrono>
#include <sstream>
#include <string>

#if defined(OS_WIN)
#include <windows.h>
#else
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace base {
uint64_t GetTaskCurrentThreadId() {
  // std::ostringstream stream;
  // stream << std::this_thread::get_id();
  // std::string stid = stream.str();
  // uint64_t tid = std::stoull(stid);
  // return tid;
#if defined(OS_WIN)
  return GetCurrentThreadId();
#else
  return syscall(SYS_gettid);
#endif
}

uint32_t GetTaskCurrentProcessId() {
#if defined(OS_WIN)
  return GetCurrentProcessId();
#else
  return getpid();
#endif
}

std::string GetFileBaseName(const std::string& path) {
  size_t lastPos = path.find_last_of("\\/");
  if (lastPos != std::string::npos) {
    return path.substr(lastPos + 1);
  }

  return path;
}

int64_t GetMillSecondsTimestamp() {
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}


}  // namespace base
