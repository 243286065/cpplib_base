#include "base/base_export.h"

#include <cstdint>
#include <string>

namespace base
{

BASE_EXPORT uint32_t Crc32(const std::string& input);

BASE_EXPORT bool Crc32(const std::string& file_path, uint32_t& crc);

} // namespace base
