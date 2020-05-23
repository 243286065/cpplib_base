#ifndef SRC_BASE_TIMER_DELAY_TIMER_H_
#define SRC_BASE_TIMER_DELAY_TIMER_H_

#include <mutex>
#include <atomic>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/thread/thread.h"

namespace base {

class DelayTimer {
 public:
  DelayTimer(const TimeDelta& delta);
  ~DelayTimer();

  void SyncBlockWait(const OnceCallback& callback = nullptr);
  void SyncNoBlockWait(const OnceCallback& callback = nullptr);
  void AsyncWait(const OnceCallback& callback);

  void Cancle();

 private:
  void InitThread();

  std::mutex mutex_;
  std::unique_ptr<Thread> thread_ = nullptr;
  TimeDelta delay_;

  DISALLOW_COPY_AND_ASSIGN(DelayTimer);
};

}  // namespace base

#endif  // SRC_BASE_TIMER_DELAY_TIMER_H_