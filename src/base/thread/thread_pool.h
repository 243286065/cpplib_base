#ifndef SRC_BASE_THREAD_THREAD_POOL_H_
#define SRC_BASE_THREAD_THREAD_POOL_H_

#include <atomic>
#include <list>
#include <memory>
#include <mutex>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/thread/task_queue.h"

namespace base {

class Thread;

class BASE_EXPORT ThreadPool {
 public:
  ThreadPool(const size_t size);
  ~ThreadPool();

  void Start();
  void Stop();

  void PostTask(const OnceCallback& task);

 private:
  void ConsumeTaskInternal();

  std::list<std::unique_ptr<Thread>> work_thread_list_;

  size_t thread_pool_size_;
  std::atomic_bool is_stopped_;
  std::mutex mutex_;

  TaskQueue task_queue_;
  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_THREAD_POOL_H_
