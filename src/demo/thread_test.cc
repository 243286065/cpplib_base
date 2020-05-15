#include "src/base/thread/thread.h"
#include "src/base/thread/utils.h"

#include <iostream>

void printOnIO() {
  std::cout << "--------------io thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
}

void printOnMain() {
  std::cout << "--------------main thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
}

void printOnUi(base::MessageLoop* main_loop) {
  std::cout << "--------------ui thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
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
    std::cout << "-------sum-------" << sum << std::endl;
}

void test() {
    std::cout << "------------------------------START------------------------------------------" << std::endl;
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

  // loop.PostTask(printOnMain);
  // thread_io.PostTaskAndReply(printOnIO, printOnMain);
  loop.RunLoop();
  std::cout << "------------------------------END------------------------------------------" << std::endl;
}

int main() {
  while(true) {
      test();
  }
  //   std::this_thread::sleep_for(std::chrono::seconds(10));
  return 0;
}