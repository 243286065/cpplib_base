#include "src/base/thread/message_loop_manager.h"

#include "src/base/thread/message_loop.h"
#include "src/base/thread/utils.h"

namespace base {

MessageLoop* MessageLoopManager::GetMessageLoop(uint64_t thread_id) {
  std::lock_guard<std::mutex> lock(loop_hashmap_mutex_);
  if (loop_hashmap_.find(thread_id) != loop_hashmap_.end()) {
    return loop_hashmap_[thread_id];
  } else {
    return nullptr;
  }
}

void MessageLoopManager::RegisterMessageLoop(MessageLoop* thread) {
  auto tid = GetCurrentThreadId();
  std::lock_guard<std::mutex> lock(loop_hashmap_mutex_);
  loop_hashmap_[tid] = thread;
}

void MessageLoopManager::UnRegisterMessageLoop(uint64_t thread_id) {
  std::lock_guard<std::mutex> lock(loop_hashmap_mutex_);
  if (loop_hashmap_.find(thread_id) != loop_hashmap_.end()) {
    loop_hashmap_[thread_id] = nullptr;
    loop_hashmap_.erase(thread_id);
  }
}
}  // namespace base
