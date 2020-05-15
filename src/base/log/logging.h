#ifndef SRC_BASE_LOG_LOGGING_H_
#define SRC_BASE_LOG_LOGGING_H_

#include <functional>
#include <sstream>

#include "src/base/base_export.h"
#include "src/base/macros.h"

namespace logging {

enum LogSeverity {
  LOG_DEBUG = -1,
  LOG_INFO = 0,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL
};

enum LoggingDestination {
  LOG_NONE,
  LOG_ONLY_TO_FILE,
  LOG_ONLY_TO_SYSTEM_DEBUG_LOG,
  LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG
};

// On startup, should we delete or append to an existing log file (if any)?
// Defaults to APPEND_TO_OLD_LOG_FILE.
enum OldFileDeletionState { DELETE_OLD_LOG_FILE, APPEND_TO_OLD_LOG_FILE };

typedef std::function<
    bool(int severity, const char* file, const int line, const char* message)>
    LogMessageHandlerFunction;

BASE_EXPORT void SetLogMessageHandler(LogMessageHandlerFunction handler);
BASE_EXPORT LogMessageHandlerFunction GetLogMessageHandler();

BASE_EXPORT void SetMinLogLevel(LogSeverity severity);
BASE_EXPORT LogSeverity GetMinLogLevel();

BASE_EXPORT void InitLogging(const char* log_file,
                             LoggingDestination logging_dest,
                             OldFileDeletionState delete_old);

class BASE_EXPORT LogMessage {
 public:
  LogMessage(LogSeverity severity, const char* file, const int line);
  ~LogMessage();

  std::ostringstream& GetStream() { return stream_; }

 private:
  LogSeverity severity_;
  std::string severity_str_;
  std::string file_;
  int line_;
  std::ostringstream stream_;
  size_t offset_;

  DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

#define LOG(SEVERITY) \
  (logging::LogMessage(logging::LOG_##SEVERITY, __FILE__, __LINE__)).GetStream()

#define CHECK(condition)                          \
  if (!(condition)) {                               \
    LOG(FATAL) << "Check failed : " #condition "."; \
  }

#if defined(DEBUG)
#define DLOG(SEVERITY) LOG(SEVERITY)
#define DCHECK(condition) CHECK(condition)
#else
#define DLOG(SEVERITY) std::ostringstream()
#define DCHECK(condition)
#endif

}  // namespace logging

#endif  // SRC_BASE_LOG_LOGGING_H_
