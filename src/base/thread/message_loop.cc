#include "base/thread/message_loop.h"

#include "base/thread/closure.h"
#include "base/thread/message_loop_manager.h"
#include "base/utils.h"

namespace base {

MessageLoop::MessageLoop() : is_stopped_(true) {}

MessageLoop::~MessageLoop() {}

void MessageLoop::RunLoop() {
  RunLoopInternal(nullptr);
}

void MessageLoop::RunLoopInternal(std::function<void()> callback) {
  BindToCurrentThread();
  is_stopped_ = false;

  if(callback) {
    callback();
  }

  // Run Loop
  while (true) {
    if (is_stopped_ && task_queue_.Empty()) {
      return;
    }

    const Closure task = task_queue_.PopTask();
    task.RunTask();
  }
}

void MessageLoop::Stop() {
  if (!is_stopped_) {
    PostTask([this]() {
      is_stopped_ = true;
      UnBindToCurrentThread();
    });
  }
}

void MessageLoop::PostTask(const OnceCallback& task) {
  Closure closure(task, nullptr);
  task_queue_.PushTask(closure);
}

void MessageLoop::PostTaskAndReply(const OnceCallback& task,
                                   const OnceCallback& callback) {
  Closure closure(task, callback);
  task_queue_.PushTask(closure);
}

void MessageLoop::BindToCurrentThread() {
	thread_id_ = GetTaskCurrentThreadId();
  MessageLoopManagerSingleton::GetInstance()->RegisterMessageLoop(this);
}

void MessageLoop::UnBindToCurrentThread() {
	MessageLoopManagerSingleton::GetInstance()->UnRegisterMessageLoop(
          thread_id_);
}

}  // namespace base
