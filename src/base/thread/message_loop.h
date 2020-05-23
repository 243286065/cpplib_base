#ifndef SRC_BASE_THREAD_MESSAGE_LOOP_H_
#define SRC_BASE_THREAD_MESSAGE_LOOP_H_

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/thread/task_queue.h"
#include "base/time/time.h"

namespace base {

class Thread;

class BASE_EXPORT MessageLoop {
 public:
  MessageLoop();
  ~MessageLoop();

  // Must be called on the work thread
  void RunLoop();

  // Can be called on any thread
  void Stop();
  void StopSoon();

  void PostTask(const OnceCallback& task);
  void PostTaskAndReply(const OnceCallback& task, const OnceCallback& callback);
  void PostDelayTask(const TimeDelta& delay, const OnceCallback& task);
  void PostDelayTaskAndReply(const TimeDelta& delay, const OnceCallback& task, const OnceCallback& callback);

  bool IsRunning() { return !is_stopped_; }

 private:
  void RunLoopInternal(std::function<void()> callback);

  // When using MessageLoop alone, because RunLoop will enter the loop, if you
  // want to perform some operations before RunLoop and you
  // want to avoid missing tasks thrown by other threads, please use
  // BindToCurrentThread as early as possible
  void BindToCurrentThread();
  void UnBindToCurrentThread();

  std::atomic<uint64_t> thread_id_;
  std::atomic_bool is_stopped_;
  std::atomic_bool stopped_soon_;
  TaskQueue task_queue_;

  friend Thread;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_MESSAGE_LOOP_H_
