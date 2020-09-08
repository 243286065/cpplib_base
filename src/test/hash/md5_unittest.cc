#include "base/hash/md5.h"
#include "base/log/logging.h"
#include "base/string/string_convert.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <iostream>

namespace base {

TEST(MD5Test, FastMD5) {
  EXPECT_EQ(FastMD5("base"), "593616de15330c0fb2d55e55410bf994");
}

TEST(MD5TEST, UpdateMD5_English) {
  std::string input = "beijing 2008";

  MD5 md5("bei");
  md5.update((const byte*)"jing", 4);
  md5.update((const byte*)" 2008", 5);
  EXPECT_EQ(md5.toStr(), FastMD5(input));

  MD5 md5_other;
  md5_other.update((const byte*)input.c_str(), input.size());
  EXPECT_EQ(md5_other.toStr(), FastMD5(input));
}

TEST(MD5TEST, UpdateMD5_Chinese) {
#if defined(OS_LINUX)
  MD5 md5("中华");
  std::string input = "人民共和国";
  md5.update((const byte*)input.c_str(), input.size());
  LOG(INFO) << "中华人民共和国: " << FastMD5("中华人民共和国");
  EXPECT_EQ(md5.toStr(), FastMD5("中华人民共和国"));
#endif
}

TEST(MD5TEST, FastFileMD5) {
  std::string file_name = "test.txt";
  std::string content = "1234567890sdfsfds差点fdsfse 三大碗饭";
  std::ofstream file;
  file.open(file_name);
  EXPECT_EQ(file.good(), true);

  file << content;
  file.close();

  std::string result;
  bool succ = FastFileMD5(file_name, result);
  LOG(INFO) << file_name << " : " << result;
  EXPECT_TRUE(succ);
  EXPECT_EQ(result, FastMD5(content));

  remove(file_name.c_str());
}

}  // namespace base
