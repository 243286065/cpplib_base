#ifndef SRC_BASE_JSON_JSON_H_
#define SRC_BASE_JSON_JSON_H_

#include <string>

#include "base/base_export.h"
#include "base/macros.h"
#include "base/third_party/rapidjson/include/rapidjson/document.h"

namespace base {

using Value = rapidjson::Value;

class BASE_EXPORT Json {
 public:
  Json();
  Json(const std::string& str);
  // Json(const rapidjson::Document& document);
  ~Json();

  std::string Str();

  rapidjson::Document::AllocatorType& GetAllocator() {
    return document_.GetAllocator();
  }

  bool Valid() { return valid_; }

  template <typename ValueType>
  Json& AddMember(const char* key, const ValueType& value) {
    rapidjson::Document::AllocatorType& allocator = document_.GetAllocator();
    document_.AddMember(rapidjson::StringRef(key), value, allocator);
    return *this;
  }

  Json& AddMember(const char* key, const std::string& value);

  Json& AddMember(const char* key, const char* value);

  void RemoveAllMembers();
  bool RemoveMember(const char* key);

  bool HasMember(const char* key);

  bool IsNull() const;
  bool IsFalse() const;
  bool IsTrue() const;
  bool IsBool() const;
  bool IsObject() const;
  bool IsArray() const;
  bool IsNumber() const;
  bool IsInt() const;
  bool IsUint() const;
  bool IsInt64() const;
  bool IsUint64() const;
  bool IsDouble() const;
  bool IsString() const;
  bool IsLosslessDouble() const;
  bool IsFloat() const;
  bool IsLosslessFloat() const;

  bool GetBool() const;
  const char* GetString() const;
  size_t GetStringLength() const;
  double GetDouble() const;
  int GetInt() const;
  unsigned GetUint() const;
  int64_t GetInt64() const;
  uint64_t GetUint64() const;

  Value::Array GetArray();
  Value::ConstArray GetArray() const;

  size_t MemberCount() const;

  void PushBack(Value& value);
  void PushBack(const std::string& value);
  template <typename ValueType>
  void PushBack(ValueType& value) {
    document_.PushBack(value, document_.GetAllocator());
  }

  //! Remove the last element in the array.
  /*!
      \note Constant time complexity.
  */
  Value& PopBack();

  //! Remove an element of array by iterator.
  /*!
      \param pos iterator to the element to remove
      \pre IsArray() == true && \ref Begin() <= \c pos < \ref End()
      \return Iterator following the removed element. If the iterator pos refers
     to the last element, the End() iterator is returned. \note Linear time
     complexity.
  */
  Value::ValueIterator Erase(Value::ConstValueIterator pos) {
    return Erase(pos, pos + 1);
  }

  //! Remove elements in the range [first, last) of the array.
  /*!
      \param first iterator to the first element to remove
      \param last  iterator following the last element to remove
      \pre IsArray() == true && \ref Begin() <= \c first <= \c last <= \ref
     End() \return Iterator following the last removed element. \note Linear
     time complexity.
  */
  Value::ValueIterator Erase(Value::ConstValueIterator first,
                             Value::ConstValueIterator last);

  Value::ValueIterator Begin();
  Value::ValueIterator End();
  Value::ConstValueIterator Begin() const;
  Value::ConstValueIterator End() const;

  template<typename T>
  Value& operator[](T key) {
    return document_[key];
  }

  //Need array
  Value& operator[](size_t index);
  const Value& operator[](size_t index) const;
  void Clear();
  bool Empty() const;
  size_t Capacity() const;
  size_t Size() const;

  //! Set this value as an empty array.
  Value& SetArray();

 private:
  std::string str_;
  rapidjson::Document document_;

  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(Json);
};

}  // namespace base

#endif  // SRC_BASE_JSON_JSON_H_