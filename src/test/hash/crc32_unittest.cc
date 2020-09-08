#include "base/hash/crc32.h"
#include "base/log/logging.h"
#include "base/string/string_convert.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>

namespace base {

TEST(CRC32TEST, Crc32) {
  EXPECT_EQ(Crc32(""), 0);
  EXPECT_EQ(Crc32("base"), 3233087073);
  EXPECT_EQ(Crc32("bAse"), 4177115265);
#if defined(OS_LINUX)
  EXPECT_EQ(Crc32("生成 $str 的 32 位循环冗余校验码多项式。这通常用于检查传输的数据是否完整。"), 129102744);
#endif
}

TEST(CRC32TEST, Crc32File) {
  std::string input;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1,6);
  while(input.size() < 1024 * 1024) {
    input.append(std::to_string(dis(gen)));
  }

  std::string file_name = "test.txt";
  std::ofstream file;
  file.open(file_name);
  EXPECT_EQ(file.good(), true);

  file << input;
  file.close();

  uint32_t file_crc32;
  bool succ = Crc32(file_name, file_crc32);
  LOG(INFO) << file_name << ": " << file_crc32;
  EXPECT_TRUE(succ);
  EXPECT_EQ(file_crc32, Crc32(input));

  remove(file_name.c_str());
}

}  // namespace base
