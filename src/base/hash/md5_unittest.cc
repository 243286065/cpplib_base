#include "base/hash/md5.h"
#include "gtest/gtest.h"

namespace base {

TEST(MD5Test, FastMD5) {
  EXPECT_EQ(FastMD5("base"), "593616de15330c0fb2d55e55410bf994");
  EXPECT_EQ(FastMD5("中国好声音"), "c567790fa04aee36bc73e42dbb5e4859");
}

}  // namespace base
