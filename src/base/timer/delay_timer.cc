#include "base/timer/delay_timer.h"

#include <chrono>
#include <thread>

namespace base {

DelayTimer::DelayTimer(const TimeDelta& delta) : delay_(delta) {}

DelayTimer::~DelayTimer() {}

void DelayTimer::SyncBlockWait(const OnceCallback& callback) {
  std::this_thread::sleep_for(
      std::chrono::microseconds(delay_.InMicroseconds()));

  if (callback) {
    callback();
  }
}

void DelayTimer::SyncNoBlockWait(const OnceCallback& callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!thread_) {
     InitThread();
  }

  thread_->PostDelayTaskAndReply(delay_, [](){}, callback);
}

void DelayTimer::AsyncWait(const OnceCallback& callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!thread_) {
    InitThread();
  }

  thread_->PostDelayTask(delay_, callback);
}

void DelayTimer::Cancle() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (thread_) {
    thread_->StopSoon();
    thread_.reset();
  }
}

void DelayTimer::InitThread() {
  thread_.reset(new Thread());
  thread_->Start();
}

}  // namespace base
