#ifndef SRC_BASE_FILE_FILE_PATH_CONSTANTS_H__
#define SRC_BASE_FILE_FILE_PATH_CONSTANTS_H__

#include <stddef.h>

#include "base/file/file_path.h"
#include "base/stl_util.h"

namespace base
{

#if defined(FILE_PATH_USES_WIN_SEPARATORS)
const FilePath::CharType FilePath::kSeparators[] = FILE_PATH_LITERAL("\\/");
#else  // FILE_PATH_USES_WIN_SEPARATORS
const FilePath::CharType FilePath::kSeparators[] = FILE_PATH_LITERAL("/");
#endif  // FILE_PATH_USES_WIN_SEPARATORS

const size_t FilePath::kSeparatorsLength = base::size(kSeparators);

const FilePath::CharType FilePath::kCurrentDirectory[] = FILE_PATH_LITERAL(".");
const FilePath::CharType FilePath::kParentDirectory[] = FILE_PATH_LITERAL("..");

const FilePath::CharType FilePath::kExtensionSeparator = FILE_PATH_LITERAL('.');

} // namespace base


#endif // SRC_BASE_FILE_FILE_PATH_CONSTANTS_H__