#include "base/hash/md5.h"

#include "base/third_party/md5/md5.h"

namespace base {

std::string FastMD5(const std::string& input) {
  MD5 md5(input);
  return md5.toStr();
}

}  // namespace base
