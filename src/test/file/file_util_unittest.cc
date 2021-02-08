#include "base/file/file_utils.h"
#include "base/log/logging.h"
#include "gtest/gtest.h"

namespace base {

TEST(FileUtilsTest, ReadFile) {
  std::string content = "JustTest 123456\r\n2132143 safsdf";
  std::string file_name = "test.txt";
  EXPECT_FALSE(FileExist(file_name.c_str()));
  EXPECT_TRUE(WriteFile(file_name.c_str(), content.c_str(), true));
  EXPECT_TRUE(FileExist(file_name.c_str()));
  EXPECT_EQ(GetFileSize(file_name.c_str()), content.size());

  std::string read_content;
  EXPECT_TRUE(ReadFile(file_name.c_str(), read_content));
  EXPECT_EQ(read_content, content);

  EXPECT_TRUE(ReadFile(file_name.c_str(), 3, read_content));
  EXPECT_EQ(read_content, content.substr(3));

  EXPECT_TRUE(ReadFile(file_name.c_str(), 3, 15, read_content));
  EXPECT_EQ(read_content, content.substr(3, 15));
;
  EXPECT_TRUE(ReadFile(file_name.c_str(), 3, 25, read_content));
  EXPECT_EQ(read_content, content.substr(3, 25));

  EXPECT_TRUE(ReadFile(file_name.c_str(), 3, std::numeric_limits<size_t>::max(), read_content));
  EXPECT_EQ(read_content, content.substr(3));

  DeleteFilePath(file_name.c_str());
  EXPECT_FALSE(FileExist(file_name.c_str()));
}

} // namespace base

