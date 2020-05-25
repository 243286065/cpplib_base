#include "base/json/json.h"

#include <string>

#include "base/log/logging.h"
#include "base/third_party/rapidjson/include/rapidjson/stringbuffer.h"
#include "base/third_party/rapidjson/include/rapidjson/writer.h"

namespace base {

Json::Json() {
  document_.SetObject();
}

Json::Json(const std::string& str) : str_(str) {
  // document_.SetObject();
  document_.Parse(str.c_str());

  valid_ = document_.HasParseError();

  if (valid_) {
    NOTREACHED() << "Invaild json str: " << document_.GetParseError();
  }
}

Json::~Json() {}

Json& Json::AddMember(const char* key, const std::string& value) {
  rapidjson::Document::AllocatorType& allocator = document_.GetAllocator();
  document_.AddMember(rapidjson::StringRef(key),
                      rapidjson::StringRef(value.c_str()), allocator);
  return *this;
}

Json& Json::AddMember(const char* key, const char* value) {
  rapidjson::Document::AllocatorType& allocator = document_.GetAllocator();
  document_.AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value),
                      allocator);
  return *this;
}

std::string Json::Str() {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

  document_.Accept(writer);
  return buffer.GetString();
}
}  // namespace base
