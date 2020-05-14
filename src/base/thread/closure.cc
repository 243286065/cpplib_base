#include "src/base/thread/closure.h"

#include "src/base/thread/thread.h"
#include "src/base/thread/thread_manager.h"
#include "src/base/thread/utils.h"

namespace base {
Closure::Closure(const TaskFunc& task, const TaskFunc& callback)
    : from_thread_id_(GetCurrentThreadId()),
      task_(std::move(task)),
      callback_(std::move(callback)) {}

Closure::~Closure() {}

void Closure::RunTask() const{
  if (task_) {
    task_();
  }

  RunCallback();
}

void Closure::RunCallback() const{
  // Call callback task on from thread
  if (callback_) {
    auto* thread =
        ThreadManagerSingleton::GetInstance()->GetThread(from_thread_id_);
		if(thread) {
    	thread->PostTask(callback_);
		}
  }
}

}  // namespace base
