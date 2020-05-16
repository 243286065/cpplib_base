#ifndef SRC_BASE_THREAD_THREAD_H_
#define SRC_BASE_THREAD_THREAD_H_

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/thread/message_loop.h"
#include "base/thread/task_queue.h"

namespace base {

class BASE_EXPORT Thread {
 public:
  Thread();
  Thread(const std::string& thread_name);
  ~Thread();

  std::string GetName();
  void SetName(const std::string& thread_name);

  void Start();
  void Stop();
	bool IsRunning();

  void PostTask(const OnceCallback& task);
  void PostTaskAndReply(const OnceCallback& task, const OnceCallback& callback);

 private:
  void DoStart();

  std::string name_;
  std::unique_ptr<std::thread> thread_;

  MessageLoop message_loop_;
  std::condition_variable cond_;
  std::mutex mutex_;

  std::atomic_bool is_stopped_;

  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace base

#endif  // SRC_BASE_THREAD_THREAD_H_
