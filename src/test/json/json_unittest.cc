#include "base/json/json.h"
#include "base/log/logging.h"
#include "gtest/gtest.h"

namespace base {

TEST(JsonTest, ParseJsonString) {
  base::Json json;
  json.AddMember("name", "zhangsan");
  json.AddMember("age", 12);
  json.AddMember("city", "beijing");
  json.AddMember("weight", 72.5);

  EXPECT_EQ(json.IsArray(), false);
  EXPECT_EQ(json.HasMember("age"), true);
  EXPECT_EQ(std::string(json["name"].GetString()), "zhangsan");
  EXPECT_EQ(json["age"].GetInt(), 12);
  EXPECT_EQ(std::string(json["city"].GetString()), "beijing");
  EXPECT_EQ(json["weight"].GetFloat(), 72.5);
  LOG(WARNING) << json.Str();

  std::string json_str = json.Str();
  base::Json rjson(json_str);
  EXPECT_EQ(rjson.IsArray(), false);
  EXPECT_EQ(rjson.HasMember("age"), true);
  EXPECT_EQ(std::string(rjson["name"].GetString()), "zhangsan");
  EXPECT_EQ(rjson["age"].GetInt(), 12);
  EXPECT_EQ(std::string(rjson["city"].GetString()), "beijing");
  EXPECT_EQ(rjson["weight"].GetFloat(), 72.5);

  EXPECT_EQ(rjson.RemoveMember("name"), true);
  EXPECT_EQ(rjson.RemoveMember("name"), false);
  EXPECT_EQ(rjson.HasMember("name"), false);
  LOG(WARNING) << rjson.Str();

  rjson.RemoveAllMembers();
  EXPECT_EQ(rjson.MemberCount(), 0);
  LOG(WARNING) << rjson.Str() << "-----" << rjson.IsNull();
}

TEST(JsonTest, ParseArrayJson) {
  base::Json json;
  json.SetArray();
  EXPECT_EQ(json.Empty(), true);
  EXPECT_EQ(json.Size(), 0);

  base::Value sub_value_1;
  sub_value_1.SetObject();
  sub_value_1.AddMember("name", "zhangsan", json.GetAllocator());
  sub_value_1.AddMember("age", 12, json.GetAllocator());
  sub_value_1.AddMember("city", "beijing", json.GetAllocator());
  sub_value_1.AddMember("weight", 72.5, json.GetAllocator());
  json.PushBack(sub_value_1);

  base::Value sub_value_2;
  sub_value_2.SetObject();
  sub_value_2.AddMember("name", "lisi", json.GetAllocator());
  sub_value_2.AddMember("age", 15, json.GetAllocator());
  sub_value_2.AddMember("city", "shanghai", json.GetAllocator());
  sub_value_2.AddMember("weight", 76.2, json.GetAllocator());
  json.PushBack(sub_value_2);

  EXPECT_EQ(json.Empty(), false);
  EXPECT_EQ(json.Size(), 2);

  for (auto& v : json.GetArray())
  {
    EXPECT_EQ(v.IsObject(), true);
    EXPECT_EQ(v.HasMember("name"), true);
    EXPECT_EQ(v["name"].IsString(), true);
  }

  LOG(WARNING) << json.Str();
}

}  // namespace base
