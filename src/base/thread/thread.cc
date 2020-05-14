#include "src/base/thread/thread.h"

#include "src/base/thread/closure.h"
#include "src/base/thread/thread_manager.h"
#include "src/base/thread/utils.h"

namespace base {

Thread::Thread() : Thread(std::string()) {}

Thread::Thread(const std::string& thread_name) : is_stopped_(true) {
  SetName(thread_name);
}

Thread::~Thread() {
  if (!is_stopped_) {
    Stop();
  }
}

void Thread::RunLoop() {
  if (!is_stopped_) {
    return;
  }
  is_stopped_ = false;
  thread_ = std::move(std::thread(std::bind(&Thread::Loop, this)));
}

void Thread::Stop() {
  PostTask(std::bind(&Thread::DoStop, this));

  if (thread_.joinable()) {
    thread_.join();
  }
}

void Thread::PostTask(const TaskFunc& task) {
  Closure closure(task, nullptr);
  task_queue_.PushTask(closure);
}

void Thread::PostTask(const TaskFunc& task, const TaskFunc& callback) {
  Closure closure(task, callback);
  task_queue_.PushTask(closure);
}

void Thread::Loop() {
  thread_id_ = GetCurrentThreadId();
  ThreadManagerSingleton::GetInstance()->RegisterCurrentThread(this);

  // Run Loop
  while (true) {
    if (is_stopped_ && task_queue_.Empty()) {
      break;
    }

    const Closure task = task_queue_.PopTask();
    task.RunTask();
  }
}

void Thread::DoStop() {
  is_stopped_ = true;
  ThreadManagerSingleton::GetInstance()->UnRegisterThread(thread_id_);
}

}  // namespace base
