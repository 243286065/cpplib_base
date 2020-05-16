#ifndef SRC_BASE_THREAD_CLOSURE_H_
#define SRC_BASE_THREAD_CLOSURE_H_

#include <functional>
#include <memory>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"

namespace base {

class BASE_EXPORT Closure {
 public:
  Closure(const OnceCallback& task, const OnceCallback& callback);
  ~Closure();

  void RunTask() const;

 private:
  void RunCallback() const;

  uint64_t from_thread_id_;
  OnceCallback task_;
  OnceCallback callback_;
};

}  // namespace base

#endif  // SRC_BASE_THREAD_CLOSURE_H_
