#include "base/thread/thread.h"
#include "base/utils.h"
#include "base/log/logging.h"
#include "base/at_exit.h"
#include "base/encode/base64.h"

#include <iostream>

void printOnIO() {
  LOG(INFO) << "--------------io thread-----------------"
            << base::GetTaskCurrentThreadId();
}

void printOnMain() {
  LOG(INFO) << "--------------main thread-----------------"
            << base::GetTaskCurrentThreadId();
}

void printOnUi(base::MessageLoop* main_loop) {
  LOG(INFO) << "--------------ui thread-----------------"
            << base::GetTaskCurrentThreadId();
  // main_loop->PostTask(printOnMain);
  main_loop->Stop();
}

int count(int start, int end) {
    int sum = 0;
    for(int i = start; i<= end; i++ ) {
        sum += i;
    }

    return sum;
}

void printResult(int sum) {
    LOG(INFO) << "-------sum-------" << sum;
}

void test_thread() {
    LOG(WARNING) << "------------------------------START------------------------------------------";
    base::Thread thread_ui;

  base::Thread thread_io;

  base::MessageLoop loop;
  //单独使用MessageLoop,由于RunLoop会进入循环,因此如果想在RunLoop之间执行一些操作的同时,还要防止遗漏其他线程抛过来的任务,请尽早使用BindToCurrentThread.
  loop.BindToCurrentThread();

  thread_io.Start();
  thread_ui.Start();

    // while(!thread_io.IsRunning() || !thread_ui.IsRunning()) {

    // }
  //   thread_io.PostTask(printOnIO);
  //   thread_ui.PostTaskAndReply(std::bind(printOnUi, &loop));
  //   thread_io.PostTask(printOnIO);
  //   thread_ui.PostTaskAndReply(std::bind(printOnUi, &loop));
  //auto test = std::bind(count, 1, 100);
  std::function<int()> test = std::bind(count, 1, 100);
  std::function<void(int)> reply = printResult;

  thread_io.PostTaskAndReply(printOnIO, std::bind(printOnUi, &loop));
  // thread_ui.PostTaskAndReply(printOnUi, printOnMain);
  thread_ui.PostTask(std::bind(printResult, 100));

  // loop.PostTask(printOnMain);
  // thread_io.PostTaskAndReply(printOnIO, printOnMain);
  loop.RunLoop();
  LOG(INFO) << "------------------------------END------------------------------------------";
  //LOG(FATAL) << 123;
  LOG(DEBUG) << 12345;
  DLOG(WARNING) << "---DLOG----";
  //CHECK(1 == 2);
  DCHECK(true);
}


void test_atexit() {
    base::AtExitManager exit_manager;

  base::AtExitManager::RegisterCallback(std::bind(printResult, 100));
  base::AtExitManager::RegisterCallback(std::bind(printResult, 101));
  base::AtExitManager::RegisterCallback(std::bind(printResult, 102));

  {
      base::AtExitManager exit_manager_2;
      base::AtExitManager::RegisterCallback(std::bind(printResult, 103));
      base::AtExitManager::RegisterCallback(std::bind(printResult, 104));
      base::AtExitManager::RegisterCallback(std::bind(printResult, 105));
  }

  base::AtExitManager exit_manager_3;
  base::AtExitManager::RegisterCallback(std::bind(printResult, 107));
  base::AtExitManager::RegisterCallback(std::bind(printResult, 108));
  base::AtExitManager::RegisterCallback(std::bind(printResult, 109));
}

void test_base64() {
  std::string input = "121321321asdadadsadsad";
  std::string result = base::Base64Encode(input);
  std::string output;
  bool valid = base::Base64Decode(result, &output);
  LOG(WARNING) << input;
  LOG(WARNING) << result;
  LOG(WARNING) << valid << "-----" << output;
}


int main() {


  // logging::InitLogging("", logging::LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG, logging::APPEND_TO_OLD_LOG_FILE);
  // while(true) {
  //     test_thread();
  // }
  //   std::this_thread::sleep_for(std::chrono::seconds(10));
  test_base64();
  return 0;
}