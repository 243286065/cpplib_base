#ifndef BASE_SRC_BASE_THREAD_THREAD_H_
#define BASE_SRC_BASE_THREAD_THREAD_H_

#include <atomic>
#include <mutex>
#include <thread>
#include <functional>

#include "src/base/base_export.h"
#include "src/base/callback.h"
#include "src/base/macros.h"
#include "src/base/thread/message_loop.h"
#include "src/base/thread/task_queue.h"

namespace base {

class BASE_EXPORT Thread {
 public:
  Thread();
  Thread(const std::string& thread_name);
  ~Thread();

  std::string GetName() const { return name_; }
  void SetName(const std::string& thread_name) { name_ = thread_name; }

  void Start();
  void Stop();

  void PostTask(const OnceCallback& task);
  void PostTask(const OnceCallback& task, const OnceCallback& callback);

 private:
  std::string name_;
  std::unique_ptr<std::thread> thread_;

	MessageLoop message_loop_;

  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace base

#endif  // BASE_SRC_BASE_THREAD_THREAD_H_