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

int main() {
  base::Thread thread_ui;

  base::Thread thread_io;

  base::MessageLoop loop;

  thread_io.Start();
  thread_ui.Start();

  //   thread_io.PostTask(printOnIO);
  //   thread_ui.PostTask(std::bind(printOnUi, &loop));
  //   thread_io.PostTask(printOnIO);
  //   thread_ui.PostTask(std::bind(printOnUi, &loop));

  thread_io.PostTask(printOnIO, std::bind(printOnUi, &loop));
  // thread_ui.PostTask(printOnUi, printOnMain);

  // loop.PostTask(printOnMain);
  // thread_io.PostTask(printOnIO, printOnMain);
  loop.RunLoop();

  //   std::this_thread::sleep_for(std::chrono::seconds(10));
  return 0;
}