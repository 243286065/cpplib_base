#include "src/base/thread/task_queue.h"

namespace base {

TaskQueue::TaskQueue() {}

TaskQueue::~TaskQueue() {}

void TaskQueue::PushTask(const Closure& task) {
  std::lock_guard<std::mutex> lock(mutex_);
  task_queue_.push(task);

  if (task_queue_.size() == 1) {
    // notify
    cond_.notify_one();
  }
}

const Closure TaskQueue::PopTask() {
	std::unique_lock<std::mutex> lock(mutex_);
	while(task_queue_.empty()) {
		cond_.wait(lock);
	}

	auto task = task_queue_.front();
	task_queue_.pop();

	return task;
}

bool TaskQueue::Empty() {
	std::lock_guard<std::mutex> lock(mutex_);
	return task_queue_.empty();
}

}  // namespace base
