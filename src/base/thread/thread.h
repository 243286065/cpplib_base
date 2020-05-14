#ifndef BASE_SRC_BASE_THREAD_THREAD_H_
#define BASE_SRC_BASE_THREAD_THREAD_H_

#include <atomic>
#include <mutex>
#include <thread>
#include <functional>

#include "src/base/base_export.h"
#include "src/base/macros.h"
#include "src/base/thread/task_queue.h"

namespace base {

class BASE_EXPORT Thread {
 public:
  typedef std::function<void()> TaskFunc;
  Thread();
  Thread(const std::string& thread_name);
  ~Thread();

  std::string GetName() const { return name_; }
  void SetName(const std::string& thread_name) { name_ = thread_name; }

  void RunLoop();
  void Stop();

  void PostTask(const TaskFunc& task);
  void PostTask(const TaskFunc& task, const TaskFunc& callback);

 private:
  void Loop();

	void DoStop();

  std::string name_;
  std::atomic<uint64_t> thread_id_;
  std::atomic_bool is_stopped_;
  std::thread thread_;
	TaskQueue task_queue_;

  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace base

#endif  // BASE_SRC_BASE_THREAD_THREAD_H_