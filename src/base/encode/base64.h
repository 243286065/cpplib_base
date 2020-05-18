#ifndef SRC_BASE_ENCODE_BASE64_H_
#define SRC_BASE_ENCODE_BASE64_H_

#include <string>

#include "base/base_export.h"

namespace base
{

BASE_EXPORT std::string Base64Encode(const std::string& input);

BASE_EXPORT bool Base64Decode(const std::string& input, std::string* output) ;

} // namespace base


#endif	// SRC_BASE_ENCODE_BASE64_H_