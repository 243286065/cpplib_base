#include "base/time/time.h"

#include <chrono>
#include <iomanip>

#include "base/log/logging.h"

namespace base {

const int64_t kHoursPerDay = 24;
const int64_t kSecondsPerMinute = 60;
const int64_t kSecondsPerHour = 60 * kSecondsPerMinute;
const int64_t kMillisecondsPerSecond = 1000;
const int64_t kMillisecondsPerDay =
    kMillisecondsPerSecond * 60 * 60 * kHoursPerDay;
const int64_t kMicrosecondsPerMillisecond = 1000;
const int64_t kMicrosecondsPerSecond =
    kMicrosecondsPerMillisecond * kMillisecondsPerSecond;
const int64_t kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
const int64_t kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
const int64_t kMicrosecondsPerDay = kMicrosecondsPerHour * kHoursPerDay;
const int64_t kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
const int64_t kNanosecondsPerMicrosecond = 1000;
const int64_t kNanosecondsPerSecond =
    kNanosecondsPerMicrosecond * kMicrosecondsPerSecond;

int64_t SaturatedAdd(int64_t value, TimeDelta delta) {
  int64_t result = 0;
  if (delta.is_max()) {
    CHECK_GT(value, std::numeric_limits<int64_t>::min());
    result = std::numeric_limits<int64_t>::max();
  } else if (delta.is_min()) {
    CHECK_LT(value, std::numeric_limits<int64_t>::max());
    result = std::numeric_limits<int64_t>::min();
  } else {
    if (value > 0 && delta.delta_ > 0 &&
        std::numeric_limits<int64_t>::max() - value < delta.delta_) {
      result = std::numeric_limits<int64_t>::max();
    } else if (value < 0 && delta.delta_ < 0 &&
               std::numeric_limits<int64_t>::min() - value > delta.delta_) {
      result = std::numeric_limits<int64_t>::min();
    } else {
      result = value + delta.delta_;
    }
  }

  return 0;
}

int64_t SaturatedSub(int64_t value, TimeDelta delta) {
  return SaturatedAdd(value, -delta);
}

TimeDelta TimeDelta::FromDays(int days) {
  return days == std::numeric_limits<int>::max()
             ? Max()
             : TimeDelta(days * kMicrosecondsPerDay);
}

TimeDelta TimeDelta::FromHours(int hours) {
  return hours == std::numeric_limits<int>::max()
             ? Max()
             : TimeDelta(hours * kMicrosecondsPerHour);
}

TimeDelta TimeDelta::FromMinutes(int minutes) {
  return minutes == std::numeric_limits<int>::max()
             ? Max()
             : TimeDelta(minutes * kMicrosecondsPerMinute);
}

TimeDelta TimeDelta::FromSeconds(int64_t secs) {
  return secs == std::numeric_limits<int>::max()
             ? Max()
             : TimeDelta(secs * kMicrosecondsPerSecond);
}

TimeDelta TimeDelta::FromMilliseconds(int64_t ms) {
  return ms == std::numeric_limits<int>::max()
             ? Max()
             : TimeDelta(ms * kMicrosecondsPerMillisecond);
}

TimeDelta TimeDelta::FromMicroseconds(int64_t us) {
  return TimeDelta(us);
}

TimeDelta TimeDelta::FromNanoseconds(int64_t ns) {
  return TimeDelta(ns / kNanosecondsPerMicrosecond);
}

TimeDelta TimeDelta::FromSecondsD(double secs) {
  return FromDouble(secs * kMicrosecondsPerSecond);
}

TimeDelta TimeDelta::FromMillisecondsD(double ms) {
  return FromDouble(ms * kMicrosecondsPerMillisecond);
}

TimeDelta TimeDelta::FromMicrosecondsD(double us) {
  return FromDouble(us);
}

TimeDelta TimeDelta::FromNanosecondsD(double ns) {
  return FromDouble(ns / kNanosecondsPerMicrosecond);
}

TimeDelta TimeDelta::FromDouble(double value) {
  return TimeDelta(static_cast<int64_t>(value));
}

TimeDelta TimeDelta::Max() {
  return TimeDelta(std::numeric_limits<int64_t>::max());
}

TimeDelta TimeDelta::Min() {
  return TimeDelta(std::numeric_limits<int64_t>::min());
}

int TimeDelta::InDays() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerDay);
}

int TimeDelta::InDaysFloored() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  int result = delta_ / kMicrosecondsPerDay;
  int64_t remainder = delta_ - (result * kMicrosecondsPerDay);
  if (remainder < 0) {
    --result;  // Use floor(), not trunc() rounding behavior.
  }
  return result;
}

int TimeDelta::InHours() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerHour);
}

int TimeDelta::InMinutes() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerMinute);
}

double TimeDelta::InSecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(delta_) / kMicrosecondsPerSecond;
}

int64_t TimeDelta::InSeconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return delta_ / kMicrosecondsPerSecond;
}

double TimeDelta::InMillisecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(delta_) / kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::InMilliseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return delta_ / kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::InMillisecondsRoundedUp() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  int64_t result = delta_ / kMicrosecondsPerMillisecond;
  int64_t remainder = delta_ - (result * kMicrosecondsPerMillisecond);
  if (remainder > 0) {
    ++result;  // Use ceil(), not trunc() rounding behavior.
  }
  return result;
}

double TimeDelta::InMicrosecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(delta_);
}

int64_t TimeDelta::InNanoseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return delta_ * kNanosecondsPerMicrosecond;
}

std::ostream& operator<<(std::ostream& os, TimeDelta time_delta) {
  return os << std::fixed << time_delta.InSecondsF() << " s";
}

int64_t Now() {
	std::chrono::microseconds ms =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}

}  // namespace base
