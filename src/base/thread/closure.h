#ifndef SRC_BASE_THREAD_CLOSURE_H_
#define SRC_BASE_THREAD_CLOSURE_H_

#include <functional>
#include <memory>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/time/time.h"

namespace base {

class TaskQueue;

class BASE_EXPORT Closure {
 public:
  Closure();
  Closure(const OnceCallback& task, const OnceCallback& callback);
  Closure(const TimeDelta& delay,
          const OnceCallback& task,
          const OnceCallback& callback);
  ~Closure();

  void RunTask() const;
  bool isNull() const {
    return is_null_;
  }

 private:
  void RunCallback() const;

  friend TaskQueue;

  uint64_t from_thread_id_;
  //Timestamp of task execution: microsmseconds
  int64_t timestamp_active_;
  OnceCallback task_;
  OnceCallback callback_;
  bool is_null_;
};

}  // namespace base

#endif  // SRC_BASE_THREAD_CLOSURE_H_
