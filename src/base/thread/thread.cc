#include "src/base/thread/thread.h"

#include "src/base/thread/closure.h"
#include "src/base/thread/utils.h"

namespace base {

Thread::Thread() : Thread(std::string()) {}

Thread::Thread(const std::string& thread_name) {
  SetName(thread_name);
}

Thread::~Thread() {
  Stop();
}

void Thread::Start() {
  if (thread_ && message_loop_.IsRunning()) {
    // Already run
    return;
  }

  thread_.reset(
      new std::thread(std::bind(&MessageLoop::RunLoop, &message_loop_)));
}

void Thread::Stop() {
  if (message_loop_.IsRunning()) {
    message_loop_.Stop();
  }

  if (thread_ && thread_->joinable()) {
    thread_->join();
  }

  thread_.reset(nullptr);
}

void Thread::PostTask(const OnceCallback& task) {
  message_loop_.PostTask(task);
}

void Thread::PostTask(const OnceCallback& task, const OnceCallback& callback) {
  message_loop_.PostTask(task, callback);
}

}  // namespace base
