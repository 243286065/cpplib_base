#include "src/base/thread/utils.h"

#include <sstream>
#include <string>
#include <thread>

namespace base {
uint64_t GetCurrentThreadId() {
  std::ostringstream stream;
  stream << std::this_thread::get_id();
  std::string stid = stream.str();
  uint64_t tid = std::stoull(stid);
  return tid;
}
}  // namespace base
