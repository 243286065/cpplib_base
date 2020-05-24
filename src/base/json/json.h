#ifndef SRC_BASE_JSON_JSON_H_
#define SRC_BASE_JSON_JSON_H_

#include <string>

#include "base/base_export.h"
#include "base/macros.h"
#include "base/third_party/rapidjson/include/rapidjson/document.h"

namespace base {

class BASE_EXPORT Json {
 public:
  Json();
  Json(const std::string& str);
  // Json(const rapidjson::Document& document);
  ~Json();

  std::string Str();

  bool Valid() { return valid_; }

  template <typename ValueType>
  void AddMember(const const char* key, const ValueType& value) {
    rapidjson::Document::AllocatorType& allocator = document_.GetAllocator();
    document_.AddMember(rapidjson::StringRef(key), value, allocator);
  }

  template <>
  void AddMember<std::string>(const char* key, const std::string& value) {
    rapidjson::Document::AllocatorType& allocator = document_.GetAllocator();
    document_.AddMember(rapidjson::StringRef(key),
                        rapidjson::StringRef(value.c_str()), allocator);
  }

  void AddMember(const char* key, const char* value);

 private:
  std::string str_;
  rapidjson::Document document_;

  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(Json);
};

}  // namespace base

#endif  // SRC_BASE_JSON_JSON_H_