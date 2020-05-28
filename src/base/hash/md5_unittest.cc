#include "base/hash/md5.h"
#include "gtest/gtest.h"
#include "base/string/string_convert.h"

namespace base {

TEST(MD5Test, FastMD5) {
  EXPECT_EQ(FastMD5("base"), "593616de15330c0fb2d55e55410bf994");
}

}  // namespace base
