#include "src/base/thread/thread.h"

#include "src/base/thread/closure.h"
#include "src/base/utils.h"

namespace base {

Thread::Thread() : Thread(std::string()) {}

Thread::Thread(const std::string& thread_name)
    : name_(thread_name), is_stopped_(true) {}

Thread::~Thread() {
  Stop();
}

std::string Thread::GetName() {
  std::lock_guard<std::mutex> lock(mutex_);
  return name_;
}

void Thread::SetName(const std::string& thread_name) {
  std::lock_guard<std::mutex> lock(mutex_);
  name_ = thread_name;
}

void Thread::Start() {
  if (thread_ && !is_stopped_) {
    // Already run
    return;
  }

  thread_.reset(new std::thread(std::bind(&Thread::DoStart, this)));

  if (is_stopped_) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (is_stopped_) {
      cond_.wait(lock);
    }
  }

  while (!message_loop_.IsRunning()) {
    std::this_thread::sleep_for(std::chrono::microseconds(5));
  }
}

void Thread::DoStart() {
  message_loop_.RunLoopInternal([this](){
    // Make sure messageloop has run
    std::lock_guard<std::mutex> lock(mutex_);
    is_stopped_ = false;
    cond_.notify_one();
  });
}

void Thread::Stop() {
  if (!is_stopped_ && message_loop_.IsRunning()) {
    message_loop_.Stop();
  }

  if (thread_ && thread_->joinable()) {
    thread_->join();
  }

  is_stopped_ = true;

  thread_.reset(nullptr);
}

bool Thread::IsRunning() {
  return !is_stopped_;
}

void Thread::PostTask(const OnceCallback& task) {
  message_loop_.PostTask(task);
}

void Thread::PostTaskAndReply(const OnceCallback& task,
                              const OnceCallback& callback) {
  message_loop_.PostTaskAndReply(task, callback);
}

}  // namespace base
