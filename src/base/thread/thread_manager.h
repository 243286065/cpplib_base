#include <memory>
#include <unordered_map>

#include "src/base/base_export.h"
#include "src/base/macros.h"
#include "src/base/singleton.h"

namespace base {

class Thread;

class BASE_EXPORT ThreadManager {
 public:
  ThreadManager() {}
  ~ThreadManager() {}

	// Get thread object by thread number
  Thread* GetThread(uint64_t thread_id);
  void RegisterCurrentThread(Thread* thread);
  void UnRegisterThread(uint64_t thread_id);

 private:
  std::unordered_map<uint64_t, Thread*> threads_hashmap_;
  std::mutex threads_hashmap_mutex_;

  DISALLOW_COPY_AND_ASSIGN(ThreadManager);
};

typedef Singleton<ThreadManager> ThreadManagerSingleton;

}  // namespace base
