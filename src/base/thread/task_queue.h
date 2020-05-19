#ifndef SRC_BASE_THREAD_TASK_QUEUE_H_
#define SRC_BASE_THREAD_TASK_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

#include "base/macros.h"
#include "base/thread/closure.h"

namespace base {

class BASE_EXPORT TaskQueue {
 public:
  TaskQueue();
  ~TaskQueue();

  void PushTask(const Closure& task);

  const Closure PopTask();

  bool Empty();

 private:
  std::mutex mutex_;
  std::queue<Closure> task_queue_;
  std::condition_variable cond_;

  DISALLOW_COPY_AND_ASSIGN(TaskQueue);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_TASK_QUEUE_H_