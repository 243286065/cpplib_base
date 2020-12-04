#ifndef SRC_BASE_THREAD_TASK_QUEUE_H_
#define SRC_BASE_THREAD_TASK_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>

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

  void Clear();

  void Close();
 private:
  struct ClouserCompare {
    bool operator()(const Closure& a, const Closure& b) {
      return a.timestamp_active_ > b.timestamp_active_;
    }
  };

  static bool CompareClouser(const Closure& task_a, const Closure& task_b);
  std::mutex mutex_;
  
  //minimum_heap
  std::priority_queue<Closure, std::vector<Closure>, TaskQueue::ClouserCompare>
      task_queue_;

  std::condition_variable cond_;
  std::atomic_bool stop_;

  DISALLOW_COPY_AND_ASSIGN(TaskQueue);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_TASK_QUEUE_H_