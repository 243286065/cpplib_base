#include "base/guid.h"
#include "gtest/gtest.h"

#include "base/log/logging.h"

namespace base {

TEST(GUIDTEST, GUIDGeneratesAllZeroes) {
  uint64_t bytes[] = {0, 0};
  std::string clientid = RandomDataToGUIDString(bytes);
  EXPECT_EQ("00000000-0000-0000-0000-000000000000", clientid);
}

TEST(GUIDTEST, GUIDGeneratesCorrectly) {
  uint64_t bytes[] = {0x0123456789ABCDEFULL, 0xFEDCBA9876543210ULL};
  std::string clientid = RandomDataToGUIDString(bytes);
  EXPECT_EQ("01234567-89ab-cdef-fedc-ba9876543210", clientid);
}

TEST(GUIDTEST, GUIDCorrectlyFormatted) {
  const int kIterations = 10;
  for (int it = 0; it < kIterations; ++it) {
    std::string guid = GenerateGUID();
    EXPECT_TRUE(IsValidGUID(guid));
    EXPECT_TRUE(IsValidGUIDOutputString(guid));
  }
}

TEST(GUIDTEST, GenerateGUID) {
  std::string guid = GenerateGUID();
  std::set<std::string> guid_set;
  int nums = 10000;
  while(nums--) {
    std::string guid = GenerateGUID();
    // LOG(WARNING) <<"guid: " << guid;
    EXPECT_EQ(guid.size(), 36);
    EXPECT_EQ(guid_set.find(guid), guid_set.end());
    EXPECT_TRUE(IsValidGUID(guid));
    guid_set.insert(guid);
  }
}

}