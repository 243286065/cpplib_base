#include "src/base/thread/message_loop.h"

#include "src/base/thread/closure.h"
#include "src/base/thread/message_loop_manager.h"
#include "src/base/thread/utils.h"

namespace base {

MessageLoop::MessageLoop() : is_stopped_(true) {}

MessageLoop::~MessageLoop() {}

void MessageLoop::RunLoop() {
  thread_id_ = GetCurrentThreadId();
  MessageLoopManagerSingleton::GetInstance()->RegisterMessageLoop(this);
  is_stopped_ = false;

  // Run Loop
  while (true) {
    if (is_stopped_ && task_queue_.Empty()) {
      break;
    }

    const Closure task = task_queue_.PopTask();
    task.RunTask();
  }
}

void MessageLoop::Stop() {
  if (!is_stopped_) {
    PostTask([this]() {
      is_stopped_ = true;
      MessageLoopManagerSingleton::GetInstance()->UnRegisterMessageLoop(
          thread_id_);
    });
  }
}

void MessageLoop::PostTask(const OnceCallback& task) {
  Closure closure(task, nullptr);
  task_queue_.PushTask(closure);
}

void MessageLoop::PostTask(const OnceCallback& task,
                           const OnceCallback& callback) {
  Closure closure(task, callback);
  task_queue_.PushTask(closure);
}

}  // namespace base
