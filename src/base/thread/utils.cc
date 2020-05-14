#include "src/base/thread/utils.h"

#include <sstream>
#include <thread>

namespace base {
uint64_t GetCurrentThreadId() {
  std::ostringstream stream;
  stream << std::this_thread::get_id();
  std::string stid = stream.str();
  unsigned long long tid = std::stoull(stid);
  return tid;
}
}  // namespace base