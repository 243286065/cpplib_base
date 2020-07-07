#include "base/hash/crc32.h"

#include "base/third_party/crc32/crc32.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

#define BUF_SIZE      256 * 1024

namespace base
{

uint32_t Crc32(const std::string& input) {
  uint32_t result = CRC::Calculate(input.c_str(), input.size(), CRC::CRC_32());
  return result;
}


bool Crc32(const std::string& file_path, uint32_t& crc) {
  std::ifstream ifs{file_path, std::ios::in | std::ios::binary};
  if (!ifs.good())
  {
    return false;
  }

  char buf[BUF_SIZE];
  bool init = false;
  while (!ifs.eof())
  {
    memset(buf, 0, BUF_SIZE);
    ifs.read(buf, BUF_SIZE);
    if (init)
    {
      crc = CRC::Calculate(buf, (size_t)ifs.gcount(), CRC::CRC_32(), crc);
    }
    else
    {
      crc = CRC::Calculate(buf, (size_t)ifs.gcount(), CRC::CRC_32());
      init = true;
    }
  }
  ifs.close();

  return true;
}

} // namespace base
