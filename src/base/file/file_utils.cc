#include "src/base/file/file_utils.h"

#include <stdio.h>
#include <string.h>

#if defined(OS_WIN)
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace base {

void DeleteFilePath(const char* file_path) {
#if defined(OS_WIN)
  DeleteFile(file_path);
#else
  unlink(file_path);
#endif
}

bool WriteFile(const char* file_path, const char* message, bool append) {
  FILE* file = NULL;
  if (append) {
    file = fopen(file_path, "a+");
  } else {
    file = fopen(file_path, "w+");
  }

	if(file == NULL) {
		return false;
	}

	size_t len = strlen(message);
	bool succ =  (len == fwrite(message,1,strlen(message),file));

	fclose(file);
	return succ;
}

}  // namespace base
