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

void Json::RemoveAllMembers() {
  document_.RemoveAllMembers();
}

bool Json::RemoveMember(const char* key) {
  return document_.RemoveMember(key);
}

std::string Json::Str() {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

  document_.Accept(writer);
  return buffer.GetString();
}

bool Json::HasMember(const char* key) {
  return document_.HasMember(key);
}

bool Json::IsNull() const {
  return document_.IsNull();
}

bool Json::IsFalse() const {
  return document_.IsFalse();
}

bool Json::IsTrue() const {
  return document_.IsTrue();
}

bool Json::IsBool() const {
  return document_.IsBool();
}

bool Json::IsObject() const {
  return document_.IsObject();
}

bool Json::IsArray() const {
  return document_.IsArray();
}

bool Json::IsNumber() const {
  return document_.IsNumber();
}

bool Json::IsInt() const {
  return document_.IsInt();
}

bool Json::IsUint() const {
  return document_.IsUint();
}

bool Json::IsInt64() const {
  return document_.IsInt64();
}

bool Json::IsUint64() const {
  return document_.IsUint64();
}

bool Json::IsDouble() const {
  return document_.IsDouble();
}

bool Json::IsString() const {
  return document_.IsString();
}

bool Json::IsLosslessDouble() const {
  return document_.IsLosslessDouble();
}

bool Json::IsFloat() const {
  return document_.IsFloat();
}

bool Json::IsLosslessFloat() const {
  return document_.IsLosslessFloat();
}

bool Json::GetBool() const {
  return document_.GetBool();
}

const char* Json::GetString() const {
  return document_.GetString();
}

size_t Json::MemberCount() const {
  return document_.MemberCount();
}

size_t Json::GetStringLength() const {
  return document_.GetStringLength();
}

double Json::GetDouble() const {
  return document_.GetDouble();
}

int Json::GetInt() const {
  return document_.GetInt();
}

unsigned Json::GetUint() const {
  return document_.GetUint();
}

int64_t Json::GetInt64() const {
  return document_.GetInt64();
}

uint64_t Json::GetUint64() const {
  return document_.GetUint64();
}

Value::Array Json::GetArray() {
  return document_.GetArray();
}

Value::ConstArray Json::GetArray() const {
  return document_.GetArray();
}

Value::ValueIterator Json::Erase(Value::ConstValueIterator first,
                                 Value::ConstValueIterator last) {
  return document_.Erase(first, last);
}

Value& Json::PopBack() {
  return document_.PopBack();
}

void Json::PushBack(Value& value) {
  document_.PushBack(value, document_.GetAllocator());
}

void Json::PushBack(const std::string& value) {
  document_.PushBack(rapidjson::StringRef(value.c_str()),
                     document_.GetAllocator());
}

Value::ValueIterator Json::Begin() {
  return document_.Begin();
}

Value::ValueIterator Json::End() {
  return document_.End();
}

Value::ConstValueIterator Json::Begin() const {
  return document_.Begin();
}

Value::ConstValueIterator Json::End() const {
  return document_.End();
}

Value& Json::operator[](size_t index) {
  return document_[index];
}

const Value& Json::operator[](size_t index) const {
  return document_[index];
}

void Json::Clear() {
  document_.Clear();
}

bool Json::Empty() const {
  return document_.Empty();
}

// Array
size_t Json::Capacity() const {
  return document_.Capacity();
}

size_t Json::Size() const {
  return document_.Size();
}

Value& Json::SetArray() {
  return document_.SetArray();
}

}  // namespace base
