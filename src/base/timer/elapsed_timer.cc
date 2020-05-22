#include "base/timer/elapsed_timer.h"

#include "base/time/time.h"

namespace base {
ElapsedTimer::ElapsedTimer() : begin_(Now()) {}

ElapsedTimer::ElapsedTimer(ElapsedTimer&& other) : begin_(other.begin_) {}

void ElapsedTimer::operator=(ElapsedTimer&& other) {
  begin_ = other.begin_;
}

TimeDelta ElapsedTimer::Elapsed() const {
  return TimeDelta::FromMicroseconds((Now() - begin_));
}

}  // namespace base
