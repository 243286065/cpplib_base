#ifndef SRC_BASE_AT_EXIT_
#define SRC_BASE_AT_EXIT_

#include <functional>
#include <mutex>
#include <stack>

#include "base/base_export.h"
#include "base/macros.h"

namespace base {

// This class provides a facility similar to the CRT atexit()
class BASE_EXPORT AtExitManager {
 public:
  typedef std::function<void()> AtExitCallback;

  AtExitManager();

	// All registered callback will be called
  ~AtExitManager();

	static void RegisterCallback(AtExitCallback func);

	static void RunCallbacksNow();

 private:
  std::mutex mutex_;
  std::stack<AtExitCallback> stack_;

	//Form a reverse list
	AtExitManager *next_manager_;

  DISALLOW_COPY_AND_ASSIGN(AtExitManager);
};

}  // namespace base

#endif  // SRC_BASE_AT_EXIT_