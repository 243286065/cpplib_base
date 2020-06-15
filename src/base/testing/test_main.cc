#include "base/log/logging.h"
#include "gtest/gtest.h"

class TestGlobalEnviroment : public testing::Environment {
 public:
  virtual void SetUp() override {
    LOG(WARNING) << "---------------TestGlobalEnviroment::SetUp----------------"
                    "---------";
  }

  virtual void TearDown() override {
    LOG(WARNING) << "---------------TestGlobalEnviroment::TearDown-------------"
                    "------------";
  }
};

// testing::Environment* const foo_env =
//     testing::AddGlobalTestEnvironment(new TestGlobalEnviroment);

int main(int argc, char* argv[]) {
  testing::AddGlobalTestEnvironment(new TestGlobalEnviroment);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}