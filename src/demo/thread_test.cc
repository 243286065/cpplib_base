#include "src/base/thread/thread.h"
#include "src/base/thread/utils.h"

#include <iostream>

void printOnIO() {
  std::cout << "--------------io thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
}

void printOnUi(base::Thread * io_thread) {
  std::cout << "--------------ui thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
    io_thread->PostTask(printOnIO);
}

void printOnMain() {
  std::cout << "--------------main thread-----------------"
            << base::GetCurrentThreadId() << std::endl;
}

int main() {
  base::Thread thread_ui;

  base::Thread thread_io;

  thread_io.RunLoop();
  thread_ui.RunLoop();

  thread_io.PostTask(printOnIO);
  thread_ui.PostTask(std::bind(printOnUi, &thread_io));
  thread_io.PostTask(printOnIO);
  thread_ui.PostTask(std::bind(printOnUi, &thread_io));

  thread_io.PostTask(printOnIO, std::bind(printOnUi, &thread_io));
  //thread_ui.PostTask(printOnUi, printOnMain);

  std::this_thread::sleep_for(std::chrono::seconds(10));
  return 0;
}