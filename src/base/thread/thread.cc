#include "base/thread/thread.h"

#include "base/log/logging.h"
#include "base/thread/closure.h"
#include "base/utils.h"

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
}

void Thread::DoStart() {
  message_loop_ = std::make_shared<MessageLoop>();
  message_loop_->RunLoopInternal([this]() {
    // Make sure messageloop has run
    std::lock_guard<std::mutex> lock(mutex_);
    is_stopped_ = false;
    cond_.notify_one();
  });
}

void Thread::Stop() {
  if (!is_stopped_ && message_loop_ && message_loop_->IsRunning()) {
    message_loop_->Stop();
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
  if (!message_loop_) {
    NOTREACHED() << "Thread has not start work";
    return;
  }
  message_loop_->PostTask(task);
}

void Thread::PostTaskAndReply(const OnceCallback& task,
                              const OnceCallback& callback) {
  if (!message_loop_) {
    NOTREACHED() << "Thread has not start work";
    return;
  }
  message_loop_->PostTaskAndReply(task, callback);
}

}  // namespace base
