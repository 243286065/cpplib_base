#include "src/base/log/logging.h"

#include "src/base/file/file_utils.h"
#include "src/base/utils.h"

#include <string.h>

#include <chrono>
#include <iomanip>
#include <mutex>

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_LINUX)
#include <sys/time.h>
#include <unistd.h>
#endif

namespace logging {

std::string GetDefultLogFile() {
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y%m%d%H%M%S");
  std::string str_time = ss.str();
  return "debug_" + str_time + ".log";
}

static LogMessageHandlerFunction g_log_message_handler = nullptr;
static LoggingDestination g_logging_destination = LOG_ONLY_TO_SYSTEM_DEBUG_LOG;
static std::string g_log_file_name = GetDefultLogFile();

#if defined(DEBUG)
static LogSeverity g_minLoglevel = LOG_DEBUG;
#elif defined(RELEASE)
static LogSeverity g_minLoglevel = LOG_WARNING;
#else
static LogSeverity g_minLoglevel = LOG_INFO;
#endif

void SetLogMessageHandler(LogMessageHandlerFunction handler) {
  g_log_message_handler = handler;
}

LogMessageHandlerFunction GetLogMessageHandler() {
  return g_log_message_handler;
}

void SetMinLogLevel(LogSeverity severity) {
  g_minLoglevel = severity;
}

LogSeverity GetMinLogLevel() {
  return g_minLoglevel;
}

void InitLogging(const char* log_file,
                 LoggingDestination logging_dest,
                 OldFileDeletionState delete_old) {
  g_log_file_name = std::string(log_file);
  if (g_log_file_name.empty()) {
    g_log_file_name = GetDefultLogFile();
  }

  g_logging_destination = logging_dest;
  if (delete_old == DELETE_OLD_LOG_FILE) {
    base::DeleteFilePath(log_file);
  }
}

LogMessage::LogMessage(LogSeverity severity, const char* file, const int line)
    : file_(base::GetFileBaseName(file)), line_(line), severity_(severity) {
  switch (severity_) {
    case LOG_DEBUG:
      severity_str_ = "DEBUG";
      break;
    case LOG_INFO:
      severity_str_ = "INFO";
      break;
    case LOG_WARNING:
      severity_str_ = "WARNING";
      break;
    case LOG_ERROR:
      severity_str_ = "ERROR";
      break;
    case LOG_FATAL:
      severity_str_ = "FATAL";
      break;
    default:
      severity_str_ = "UNKNOW";
  }

  stream_ << '[' << base::GetTaskCurrentProcessId() << ":"
          << base::GetTaskCurrentThreadId() << ":";

#if defined(OS_WIN)
  SYSTEMTIME local_time;
  GetLocalTime(&local_time);
  stream_ << std::setfill('0') << std::setw(2) << local_time.wMonth
          << std::setw(2) << local_time.wDay << '/' << std::setw(2)
          << local_time.wHour << std::setw(2) << local_time.wMinute
          << std::setw(2) << local_time.wSecond << '.' << std::setw(3)
          << local_time.wMilliseconds << ':';
#elif defined(OS_LINUX)
  timeval tv;
  gettimeofday(&tv, nullptr);
  time_t t = tv.tv_sec;
  struct tm local_time;
  localtime_r(&t, &local_time);
  struct tm* tm_time = &local_time;
  stream_ << std::setfill('0') << std::setw(2) << 1 + tm_time->tm_mon
          << std::setw(2) << tm_time->tm_mday << '/' << std::setw(2)
          << tm_time->tm_hour << std::setw(2) << tm_time->tm_min << std::setw(2)
          << tm_time->tm_sec << '.' << std::setw(6) << tv.tv_usec << ':';
#else
// Unsupported platform
#endif

  stream_ << severity_str_ << ":" << file_ << "(" << line_ << ")] ";
  offset_ = stream_.str().size();
}

LogMessage::~LogMessage() {
  if (severity_ < g_minLoglevel) {
    return;
  }
  stream_ << std::endl;
  std::string message = stream_.str();

  if (g_log_message_handler &&
      g_log_message_handler(severity_, file_.c_str(), line_,
                            message.substr(offset_).c_str())) {
    return;
  }

  if (g_logging_destination == LOG_ONLY_TO_FILE ||
      g_logging_destination == LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG) {
    // write to file
    base::WriteFile(g_log_file_name.c_str(), message.c_str(), true);
  }

  if (g_logging_destination == LOG_ONLY_TO_SYSTEM_DEBUG_LOG ||
      g_logging_destination == LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG) {
// Print to console
#if defined(OS_WIN)
    OutputDebugStringA(message.c_str());
#endif
    fwrite(message.c_str(), message.size(), 1, stderr);
    fflush(stderr);
  }

  if (severity_ == LOG_FATAL) {
    abort();
  }
}

}  // namespace logging
