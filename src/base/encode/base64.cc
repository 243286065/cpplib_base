// Copy from chromium:base/base64.cc

#include "base/encode/base64.h"

#include "base/third_party/modp_b64/modp_b64.h"

namespace base {

std::string Base64Encode(const std::string& input) {
  std::string output;
  output.resize(modp_b64_encode_len(input.size()));

	// modp_b64_encode_len() returns at least 1, so output[0] is safe to use.
  const size_t output_size = modp_b64_encode(
      &(output[0]), reinterpret_cast<const char*>(input.data()), input.size());
  output.resize(output_size);
	return output;
}

bool Base64Decode(const std::string& input, std::string* output) {
	std::string temp;
	temp.resize(modp_b64_decode_len(input.size()));

	// does not null terminate result since result is binary data!
  size_t input_size = input.size();
  size_t output_size = modp_b64_decode(&(temp[0]), input.data(), input_size);
  if (output_size == MODP_B64_ERROR)
    return false;

  temp.resize(output_size);
  output->swap(temp);
  return true;
}

}  // namespace base
