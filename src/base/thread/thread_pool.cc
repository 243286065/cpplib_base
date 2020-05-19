#include "base/thread/thread_pool.h"

#include "base/log/logging.h"
#include "base/thread/closure.h"
#include "base/thread/thread.h"

namespace base {

ThreadPool::ThreadPool(const size_t size)
    : is_stopped_(true), thread_pool_size_(size) {
  for (int i = 0; i < thread_pool_size_; i++) {
    std::unique_ptr<Thread> thread;
    thread.reset(new Thread());
    work_thread_list_.emplace_back(std::move(thread));
  }
}

ThreadPool::~ThreadPool() {
  Stop();
}

void ThreadPool::Start() {
  if (is_stopped_) {
    std::lock_guard<std::mutex> lock(mutex_);
    is_stopped_ = false;

    for (auto& thread : work_thread_list_) {
      thread->Start();
      thread->PostTask(std::bind(&ThreadPool::ConsumeTaskInternal, this));
    }
  }
}

void ThreadPool::Stop() {
  if (!is_stopped_) {
    is_stopped_ = true;

    for (auto& thread : work_thread_list_) {
      thread->Stop();
    }
  }
}

void ThreadPool::PostTask(const OnceCallback& task) {
  if (!is_stopped_) {
    Closure closure(task, nullptr);
    task_queue_.PushTask(closure);
  } else {
    NOTREACHED() << "The thread pool not work";
  }
}

void ThreadPool::ConsumeTaskInternal() {
  // Run Loop
  while (true) {
    if (is_stopped_ && task_queue_.Empty()) {
      return;
    }

    const Closure task = task_queue_.PopTask();
    task.RunTask();
  }
}

}  // namespace base
