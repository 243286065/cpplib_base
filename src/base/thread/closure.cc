#include "src/base/thread/closure.h"

#include "src/base/thread/message_loop.h"
#include "src/base/thread/message_loop_manager.h"
#include "src/base/thread/utils.h"

namespace base {
Closure::Closure(const OnceCallback& task, const OnceCallback& callback)
    : from_thread_id_(GetCurrentThreadId()),
      task_(std::move(task)),
      callback_(std::move(callback)) {}

Closure::~Closure() {}

void Closure::RunTask() const {
  if (task_) {
    task_();
  }

  RunCallback();
}

void Closure::RunCallback() const {
  // Call callback task on from thread
  if (callback_) {
    auto* message_loop =
        MessageLoopManagerSingleton::GetInstance()->GetMessageLoop(
            from_thread_id_);
    if (message_loop) {
      message_loop->PostTask(callback_);
    }
  }
}

}  // namespace base