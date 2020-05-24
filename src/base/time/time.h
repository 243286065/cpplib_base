#ifndef SRC_BASE_TIME_TIME_H_
#define SRC_BASE_TIME_TIME_H_

#include <stdint.h>

#include <iosfwd>
#include <limits>

#include "base/base_export.h"

namespace base {

extern const int64_t kHoursPerDay;
extern const int64_t kSecondsPerMinute;
extern const int64_t kSecondsPerHour;
extern const int64_t kMillisecondsPerSecond;
extern const int64_t kMillisecondsPerDay;
extern const int64_t kMicrosecondsPerMillisecond;
extern const int64_t kMicrosecondsPerSecond;
extern const int64_t kMicrosecondsPerMinute;
extern const int64_t kMicrosecondsPerHour;
extern const int64_t kMicrosecondsPerDay;
extern const int64_t kMicrosecondsPerWeek;
extern const int64_t kNanosecondsPerMicrosecond;
extern const int64_t kNanosecondsPerSecond;

class TimeDelta;

// Add or subtract a TimeDelta from |value|. TimeDelta::Min()/Max() are treated
// as infinity and will always saturate the return value (infinity math applies
// if |value| also is at either limit of its spectrum). The int64_t argument and
// return value are in terms of a microsecond timebase.
BASE_EXPORT int64_t SaturatedAdd(int64_t value, TimeDelta delta);
BASE_EXPORT int64_t SaturatedSub(int64_t value, TimeDelta delta);

class BASE_EXPORT TimeDelta {
 public:
  TimeDelta() : delta_(0) {}

  // Converts units of time to TimeDeltas.
  // WARNING: Floating point arithmetic is such that FromXXXD(t.InXXXF()) may
  // not precisely equal |t|. Hence, floating point values should not be used
  // for storage.
  static TimeDelta FromDays(int days);
  static TimeDelta FromHours(int hours);
  static TimeDelta FromMinutes(int minutes);
  static TimeDelta FromSeconds(int64_t secs);
  static TimeDelta FromMilliseconds(int64_t ms);
  static TimeDelta FromMicroseconds(int64_t us);
  static TimeDelta FromNanoseconds(int64_t ns);
  static TimeDelta FromSecondsD(double secs);
  static TimeDelta FromMillisecondsD(double ms);
  static TimeDelta FromMicrosecondsD(double us);
  static TimeDelta FromNanosecondsD(double ns);

  // Returns the maximum time delta, which should be greater than any reasonable
  // time delta we might compare it to. Adding or subtracting the maximum time
  // delta to a time or another time delta has an undefined result.
  static TimeDelta Max();

  // Returns the minimum time delta, which should be less than than any
  // reasonable time delta we might compare it to. Adding or subtracting the
  // minimum time delta to a time or another time delta has an undefined result.
  static TimeDelta Min();

  // Returns true if the time delta is zero.
  constexpr bool is_zero() const { return delta_ == 0; }

  // Returns true if the time delta is the maximum/minimum time delta.
  constexpr bool is_max() const {
    return delta_ == std::numeric_limits<int64_t>::max();
  }
  constexpr bool is_min() const {
    return delta_ == std::numeric_limits<int64_t>::min();
  }

  // Returns the time delta in some unit. The InXYZF versions return a floating
  // point value. The InXYZ versions return a truncated value (aka rounded
  // towards zero, std::trunc() behavior). The InXYZFloored() versions round to
  // lesser integers (std::floor() behavior). The XYZRoundedUp() versions round
  // up to greater integers (std::ceil() behavior).
  // WARNING: Floating point arithmetic is such that FromXXXD(t.InXXXF()) may
  // not precisely equal |t|. Hence, floating point values should not be used
  // for storage.
  int InDays() const;
  int InDaysFloored() const;
  int InHours() const;
  int InMinutes() const;
  double InSecondsF() const;
  int64_t InSeconds() const;
  double InMillisecondsF() const;
  int64_t InMilliseconds() const;
  int64_t InMillisecondsRoundedUp() const;
  constexpr int64_t InMicroseconds() const { return delta_; }
  double InMicrosecondsF() const;
  int64_t InNanoseconds() const;

  // Computations with other deltas.
  TimeDelta operator+(TimeDelta other) const {
    return TimeDelta(SaturatedAdd(delta_, other));
  }
  TimeDelta operator-(TimeDelta other) const {
    return TimeDelta(SaturatedSub(delta_, other));
  }
  TimeDelta& operator+=(TimeDelta other) {
		delta_ += other.delta_;
    return *this;
  }
  TimeDelta& operator-=(TimeDelta other) {
		delta_ -= other.delta_;
		return *this;
  }

  TimeDelta operator-() const { return TimeDelta(-delta_); }

  // Comparison operators.
  bool operator==(TimeDelta other) const {
    return delta_ == other.delta_;
  }
  bool operator!=(TimeDelta other) const {
    return delta_ != other.delta_;
  }
  bool operator<(TimeDelta other) const {
    return delta_ < other.delta_;
  }
  bool operator<=(TimeDelta other) const {
    return delta_ <= other.delta_;
  }
  bool operator>(TimeDelta other) const {
    return delta_ > other.delta_;
  }
  bool operator>=(TimeDelta other) const {
    return delta_ >= other.delta_;
  }

 private:
  friend int64_t SaturatedAdd(int64_t value, TimeDelta delta);
  friend int64_t SaturatedSub(int64_t value, TimeDelta delta);

  explicit TimeDelta(int64_t delta_us) : delta_(delta_us) {}

  static TimeDelta FromDouble(double value);

  int64_t delta_;
};

// For logging use only.
BASE_EXPORT std::ostream& operator<<(std::ostream& os, TimeDelta time_delta);

// Current timestamp with microseconds
BASE_EXPORT int64_t Now();

}  // namespace base

#endif  // SRC_BASE_TIME_TIME_H_