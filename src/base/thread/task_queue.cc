#include "base/thread/task_queue.h"

namespace base {

TaskQueue::TaskQueue() : stop_(false) {}

TaskQueue::~TaskQueue() {
  if(!stop_)
    Close();
}

void TaskQueue::Close() {
  std::lock_guard<std::mutex> lock(mutex_);
  stop_ = true;
  cond_.notify_all();
}

void TaskQueue::PushTask(const Closure& task) {
  std::lock_guard<std::mutex> lock(mutex_);
  task_queue_.push(task);
  cond_.notify_one();
}

const Closure TaskQueue::PopTask() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (!stop_ && task_queue_.empty()) {
    cond_.wait(lock);
  }

  if(task_queue_.empty())
    return Closure();

  auto delay = Now() - task_queue_.top().timestamp_active_;
  while(true) {
    auto delay = Now() - task_queue_.top().timestamp_active_;
    if(delay < 0) {
      cond_.wait_for(lock, std::chrono::microseconds(std::abs(delay)));
    } else {
      break;
    }
  }

  auto task = task_queue_.top();
  task_queue_.pop();

  return task;
}

bool TaskQueue::Empty() {
  std::lock_guard<std::mutex> lock(mutex_);
  return task_queue_.empty();
}

void TaskQueue::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);
  while(!task_queue_.empty()) {
    task_queue_.pop();
  }
}

}  // namespace base
