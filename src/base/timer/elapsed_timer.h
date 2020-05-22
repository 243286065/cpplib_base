#ifndef SRC_BASE_TIMER_ELAPSED_TIMER_H_
#define SRC_BASE_TIMER_ELAPSED_TIMER_H_

#include "base/base_export.h"
#include "base/macros.h"
#include "base/time/time.h"

namespace base {

class ElapsedTimer {
 public:
  ElapsedTimer();
  ElapsedTimer(ElapsedTimer&& other);
  void operator=(ElapsedTimer&& other);
  ~ElapsedTimer() = default;

  // Returns the time elapsed since object construction.
  TimeDelta Elapsed() const;

  // Returns the timestamp of the creation of this timer(ms).
  int64_t Begin() const { return begin_; }

 private:
  int64_t begin_;

  DISALLOW_COPY_AND_ASSIGN(ElapsedTimer);
};

}  // namespace base

#endif  // SRC_BASE_TIMER_ELAPSED_TIMER_H_
