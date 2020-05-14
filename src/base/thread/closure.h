#ifndef BASE_SRC_BASE_THREAD_CLOSURE_H
#define BASE_SRC_BASE_THREAD_CLOSURE_H

#include <memory>
#include <functional>

#include "src/base/base_export.h"
#include "src/base/macros.h"

namespace base {

class BASE_EXPORT Closure {
 public:
 	typedef std::function<void()> TaskFunc;

  Closure(const TaskFunc& task, const TaskFunc& callback);
  ~Closure();

  void RunTask() const;

 private:
  void RunCallback() const;

  uint64_t from_thread_id_;
  TaskFunc task_;
  TaskFunc callback_;
};

}  // namespace base

#endif  // BASE_SRC_BASE_THREAD_CLOSURE_H