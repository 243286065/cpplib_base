#ifndef SRC_BASE_THREAD_MESSAGE_LOOP_H_
#define SRC_BASE_THREAD_MESSAGE_LOOP_H_

namespace base {

class MessageLoop {
 public:
  MessageLoop();
	~MessageLoop();

	void RunLoop();

	
};

}  // namespace base

#endif  // SRC_BASE_THREAD_MESSAGE_LOOP_H_