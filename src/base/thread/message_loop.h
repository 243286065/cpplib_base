#ifndef SRC_BASE_THREAD_MESSAGE_LOOP_H_
#define SRC_BASE_THREAD_MESSAGE_LOOP_H_

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>

#include "src/base/base_export.h"
#include "src/base/callback.h"
#include "src/base/macros.h"
#include "src/base/thread/task_queue.h"

namespace base {

class BASE_EXPORT MessageLoop {
 public:
  MessageLoop();
  ~MessageLoop();

  // Must be called on the work thread
  void RunLoop();

  // Can be called on any thread
  void Stop();

  void PostTask(const OnceCallback& task);
  void PostTask(const OnceCallback& task, const OnceCallback& callback);

  bool IsRunning() { return !is_stopped_; }

 private:
  std::atomic<uint64_t> thread_id_;
  std::atomic_bool is_stopped_;
  TaskQueue task_queue_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_MESSAGE_LOOP_H_
