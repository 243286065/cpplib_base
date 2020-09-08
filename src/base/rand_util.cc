#include "base/rand_util.h"

#include <limits.h>
#include <math.h>

#include <algorithm>
#include <limits>
#include <random>

#if defined(OS_LINUX)
#include <sys/random.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#elif defined(OS_WIN)
#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#endif

#include "base/log/logging.h"

namespace base
{

uint64_t RandUint64() {
  uint64_t number;
  RandBytes(&number, sizeof(number));
  return number;
}

int RandInt(int min, int max) {
  DCHECK_LE(min, max);

  uint64_t range = static_cast<uint64_t>(max) - min + 1;
  // |range| is at most UINT_MAX + 1, so the result of RandGenerator(range)
  // is at most UINT_MAX.  Hence it's safe to cast it from uint64_t to int64_t.
  int result =
      static_cast<int>(min + static_cast<int64_t>(base::RandGenerator(range)));
  DCHECK_GE(result, min);
  DCHECK_LE(result, max);

  return result;
}

uint64_t RandGenerator(uint64_t range) {
  DCHECK_GT(range, 0u);
  // We must discard random results above this number, as they would
  // make the random generator non-uniform (consider e.g. if
  // MAX_UINT64 was 7 and |range| was 5, then a result of 1 would be twice
  // as likely as a result of 3 or 4).
  uint64_t max_acceptable_value =
      (std::numeric_limits<uint64_t>::max() / range) * range - 1;

  uint64_t value;
  do {
    value = base::RandUint64();
  } while (value > max_acceptable_value);

  return value % range;
} 

double RandDouble() {
  return BitsToOpenEndedUnitInterval(base::RandUint64());
}

double BitsToOpenEndedUnitInterval(uint64_t bits) {
  // We try to get maximum precision by masking out as many bits as will fit
  // in the target type's mantissa, and raising it to an appropriate power to
  // produce output in the range [0, 1).  For IEEE 754 doubles, the mantissa
  // is expected to accommodate 53 bits.

  static_assert(std::numeric_limits<double>::radix == 2,
                "otherwise use scalbn");
  static const int kBits = std::numeric_limits<double>::digits;
  uint64_t random_bits = bits & ((UINT64_C(1) << kBits) - 1);
  double result = ldexp(static_cast<double>(random_bits), -1 * kBits);
  DCHECK_GE(result, 0.0);
  DCHECK_LT(result, 1.0);
  return result;
}

void RandBytes(void* output, size_t output_length) {
#if defined(OS_LINUX)
  const ssize_t r = getrandom(output, output_length, 0);
  if (output_length == static_cast<size_t>(r)) {
    return;
  }
#elif defined(OS_WIN)
  char* output_ptr = static_cast<char*>(output);
  while (output_length > 0) {
    const ULONG output_bytes_this_pass = static_cast<ULONG>(std::min(
        output_length, static_cast<size_t>(std::numeric_limits<ULONG>::max())));
    const bool success =
        RtlGenRandom(output_ptr, output_bytes_this_pass) != FALSE;
    CHECK(success);
    output_length -= output_bytes_this_pass;
    output_ptr += output_bytes_this_pass;
  }
#endif
}

std::string RandBytesAsString(size_t length) {
  DCHECK_GT(length, 0u);
  std::string result(length, 0);
  RandBytes((void*)result.data(), length);
  return result;
}

} // namespace base
