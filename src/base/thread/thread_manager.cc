#include "src/base/thread/thread_manager.h"

#include "src/base/thread/thread.h"
#include "src/base/thread/utils.h"

namespace base
{

Thread* ThreadManager::GetThread(uint64_t thread_id) {
	std::lock_guard<std::mutex> lock(threads_hashmap_mutex_);
	if(threads_hashmap_.find(thread_id) != threads_hashmap_.end()) {
		return threads_hashmap_[thread_id];
	} else {
		return nullptr;
	}
}

void ThreadManager::RegisterCurrentThread(Thread* thread) {
	auto tid = GetCurrentThreadId();
	std::lock_guard<std::mutex> lock(threads_hashmap_mutex_);
	threads_hashmap_[tid] = thread;
}

void ThreadManager::UnRegisterThread(uint64_t thread_id) {
	std::lock_guard<std::mutex> lock(threads_hashmap_mutex_);
	if(threads_hashmap_.find(thread_id) != threads_hashmap_.end()) {
		threads_hashmap_[thread_id] = nullptr;
		threads_hashmap_.erase(thread_id);
	}
}
} // namespace base
