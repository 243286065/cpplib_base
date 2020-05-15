#include "src/base/thread/utils.h"

#include <sstream>
#include <string>

#if defined(OS_WIN)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace base {
uint64_t GetCurrentThreadId() {
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
}  // namespace base
