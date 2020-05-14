#ifndef BASE_SRC_BASE_THREAD_CLOSURE_H
#define BASE_SRC_BASE_THREAD_CLOSURE_H

#include <memory>
#include <functional>

#include "src/base/base_export.h"
#include "src/base/callback.h"
#include "src/base/macros.h"

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

#endif  // BASE_SRC_BASE_THREAD_CLOSURE_H