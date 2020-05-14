#ifndef SRC_BASE_SINGLETON_H_
#define SRC_BASE_SINGLETON_H_

#include <memory>
#include <mutex>

#include "src/base/base_export.h"
#include "src/base/macros.h"

namespace base {
template <typename T>
class BASE_EXPORT Singleton {
 public:
  ~Singleton() {}

  static std::shared_ptr<T> GetInstance() {
    if (!instance_) {
      std::lock_guard<std::mutex> lock(mutex_);
      if (!instance_) {
        instance_ = std::make_shared<T>();
      }
    }

    return instance_;
  }

 private:
  Singleton() {}

  static std::shared_ptr<T> instance_;
  static std::mutex mutex_;
  DISALLOW_COPY_AND_ASSIGN(Singleton);
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

template <typename T>
std::mutex Singleton<T>::mutex_;

}  // namespace base

#endif  // SRC_BASE_SINGLETON_H_
