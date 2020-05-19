#ifndef SRC_BASE_ENCODE_BASE64_H_
#define SRC_BASE_ENCODE_BASE64_H_

#include <string>

#include "base/base_export.h"

namespace base
{

BASE_EXPORT std::string Base64Encode(const std::string& input);

BASE_EXPORT bool Base64Decode(const std::string& input, std::string* output) ;

// Return encode/decode string length, In case finally length is more than [modp_b64_encode_len] 10b.
BASE_EXPORT size_t Base64EncodeLength(const std::string& input);
BASE_EXPORT size_t Base64DecodeLength(const std::string& input);

} // namespace base


#endif	// SRC_BASE_ENCODE_BASE64_H_