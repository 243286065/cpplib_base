#ifndef SRC_BASE_FILE_FILE_PATH_H__
#define SRC_BASE_FILE_FILE_PATH_H__

#include "base/base_export.h"
#include "base/compiler_special.h"
#include "base/macros.h"

#include <string>
#include <vector>

// Macros for string literal initialization of FilePath::CharType[].
#if defined(OS_WIN)
#define FILE_PATH_LITERAL(x) L##x
#elif defined(OS_LINUX)
#define FILE_PATH_LITERAL(x) x
#endif  // OS_WIN

// Windows-style drive letter support and pathname separator characters can be
// enabled and disabled independently, to aid testing.  These #defines are
// here so that the same setting can be used in both the implementation and
// in the unit test.
#if defined(OS_WIN)
#define FILE_PATH_USES_DRIVE_LETTERS
#define FILE_PATH_USES_WIN_SEPARATORS
#endif  // OS_WIN

namespace base {

class BASE_EXPORT FilePath {
 public:
#if defined(OS_WIN)
  typedef std::wstring StringType;
#else
  typedef std::string StringType;
#endif  // OS_WIN

  typedef StringType::value_type CharType;
  static const CharType kSeparators[];
  static const size_t kSeparatorsLength;
  // A special path component meaning "this directory"
  static const CharType kCurrentDirectory[];

  // A special path component meaning "the parent directory"
  static const CharType kParentDirectory[];

  // The character used to identify a file extension
  static const CharType kExtensionSeparator;

  FilePath();
  FilePath(const StringType& path);
  FilePath(const FilePath& that);
  // Constructs FilePath with the contents of |that|, which is left in valid but
  // unspecified state.
  FilePath(FilePath&& that) noexcept;
  FilePath& operator=(const FilePath& that);
  // Constructs FilePath with the contents of |that|, which is left in valid but
  // unspecified state.
  FilePath& operator=(FilePath&& that) noexcept;
  ~FilePath();

  bool operator==(const FilePath& that) const;
  bool operator!=(const FilePath& that) const;
  bool operator<(const FilePath& that) const { return path_ < that.path_; }

  const StringType& value() const { return path_; }

  bool empty() const { return path_.empty(); }

  void clear() { path_.clear(); }

  // Returns true if |character| is in kSeparators.
  static bool IsSeparator(CharType character);

  // Returns a vector of all of the components of the provided path. It is
  // equivalent to calling DirName().value() on the path's root component,
  // and BaseName().value() on each child component.
  //
  // To make sure this is lossless so we can differentiate absolute and
  // relative paths, the root slash will be included even though no other
  // slashes will be. The precise behavior is:
  //
  // Posix:  "/foo/bar"  ->  [ "/", "foo", "bar" ]
  // Windows:  "C:\foo\bar"  ->  [ "C:", "\\", "foo", "bar" ]
  void GetComponents(std::vector<FilePath::StringType>* components) const;

  // Returns true if this FilePath is a parent or ancestor of the |child|.
  // Absolute and relative paths are accepted i.e. /foo is a parent to /foo/bar,
  // and foo is a parent to foo/bar. Any ancestor is considered a parent i.e. /a
  // is a parent to both /a/b and /a/b/c.  Does not convert paths to absolute,
  // follow symlinks or directory navigation (e.g. ".."). A path is *NOT* its
  // own parent.
  bool IsParent(const FilePath& child) const;

  // If IsParent(child) holds, appends to path (if non-NULL) the
  // relative path to child and returns true.  For example, if parent
  // holds "/Users/johndoe/Library/Application Support", child holds
  // "/Users/johndoe/Library/Application Support/Google/Chrome/Default", and
  // *path holds "/Users/johndoe/Library/Caches", then after
  // parent.AppendRelativePath(child, path) is called *path will hold
  // "/Users/johndoe/Library/Caches/Google/Chrome/Default".  Otherwise,
  // returns false.
  bool AppendRelativePath(const FilePath& child, FilePath* path) const;

  // Returns a FilePath corresponding to the directory containing the path
  // named by this object, stripping away the file component.  If this object
  // only contains one component, returns a FilePath identifying
  // kCurrentDirectory.  If this object already refers to the root directory,
  // returns a FilePath identifying the root directory. Please note that this
  // doesn't resolve directory navigation, e.g. the result for "../a" is "..".
  FilePath DirName() const WARN_UNUSED_RESULT;

  // Returns a FilePath corresponding to the last path component of this
  // object, either a file or a directory.  If this object already refers to
  // the root directory, returns a FilePath identifying the root directory;
  // this is the only situation in which BaseName will return an absolute path.
  FilePath BaseName() const WARN_UNUSED_RESULT;

  // Returns ".jpg" for path "C:\pics\jojo.jpg", or an empty string if
  // the file has no extension.  If non-empty, Extension() will always start
  // with precisely one ".".  The following code should always work regardless
  // of the value of path.  For common double-extensions like .tar.gz and
  // .user.js, this method returns the combined extension.  For a single
  // component, use FinalExtension().
  // new_path = path.RemoveExtension().value().append(path.Extension());
  // ASSERT(new_path == path.value());
  // NOTE: this is different from the original file_util implementation which
  // returned the extension without a leading "." ("jpg" instead of ".jpg")
  StringType Extension() const WARN_UNUSED_RESULT;

  // Returns the path's file extension, as in Extension(), but will
  // never return a double extension.
  //
  // TODO(davidben): Check all our extension-sensitive code to see if
  // we can rename this to Extension() and the other to something like
  // LongExtension(), defaulting to short extensions and leaving the
  // long "extensions" to logic like base::GetUniquePathNumber().
  StringType FinalExtension() const WARN_UNUSED_RESULT;

  // Returns "C:\pics\jojo" for path "C:\pics\jojo.jpg"
  // NOTE: this is slightly different from the similar file_util implementation
  // which returned simply 'jojo'.
  FilePath RemoveExtension() const WARN_UNUSED_RESULT;

  // Removes the path's file extension, as in RemoveExtension(), but
  // ignores double extensions.
  FilePath RemoveFinalExtension() const WARN_UNUSED_RESULT;

  // Inserts |suffix| after the file name portion of |path| but before the
  // extension.  Returns "" if BaseName() == "." or "..".
  // Examples:
  // path == "C:\pics\jojo.jpg" suffix == " (1)", returns "C:\pics\jojo (1).jpg"
  // path == "jojo.jpg"         suffix == " (1)", returns "jojo (1).jpg"
  // path == "C:\pics\jojo"     suffix == " (1)", returns "C:\pics\jojo (1)"
  // path == "C:\pics.old\jojo" suffix == " (1)", returns "C:\pics.old\jojo (1)"
  FilePath InsertBeforeExtension(const StringType& suffix) const
      WARN_UNUSED_RESULT;

  // Adds |extension| to |file_name|. Returns the current FilePath if
  // |extension| is empty. Returns "" if BaseName() == "." or "..".
  FilePath AddExtension(const StringType& extension) const WARN_UNUSED_RESULT;

  // Replaces the extension of |file_name| with |extension|.  If |file_name|
  // does not have an extension, then |extension| is added.  If |extension| is
  // empty, then the extension is removed from |file_name|.
  // Returns "" if BaseName() == "." or "..".
  FilePath ReplaceExtension(const StringType& extension) const
      WARN_UNUSED_RESULT;

  // Returns true if the file path matches the specified extension. The test is
  // case insensitive. Don't forget the leading period if appropriate.
  bool MatchesExtension(const StringType& extension) const;

  // Returns a FilePath by appending a separator and the supplied path
  // component to this object's path.  Append takes care to avoid adding
  // excessive separators if this object's path already ends with a separator.
  // If this object's path is kCurrentDirectory, a new FilePath corresponding
  // only to |component| is returned.  |component| must be a relative path;
  // it is an error to pass an absolute path.
  FilePath Append(const StringType& component) const WARN_UNUSED_RESULT;
  FilePath Append(const FilePath& component) const WARN_UNUSED_RESULT;

  // Returns true if this FilePath contains an absolute path.  On Windows, an
  // absolute path begins with either a drive letter specification followed by
  // a separator character, or with two separator characters.  On POSIX
  // platforms, an absolute path begins with a separator character.
  bool IsAbsolute() const;

  // Returns true if the patch ends with a path separator character.
  bool EndsWithSeparator() const WARN_UNUSED_RESULT;

  // Returns a copy of this FilePath that ends with a trailing separator. If
  // the input path is empty, an empty FilePath will be returned.
  FilePath AsEndingWithSeparator() const WARN_UNUSED_RESULT;

  // Returns a copy of this FilePath that does not end with a trailing
  // separator.
  FilePath StripTrailingSeparators() const WARN_UNUSED_RESULT;

  // Returns true if this FilePath contains an attempt to reference a parent
  // directory (e.g. has a path component that is "..").
  bool ReferencesParent() const;

  // Return a Unicode human-readable version of this path.
  // Warning: you can *not*, in general, go from a display name back to a real
  // path.  Only use this when displaying paths to users, not just when you
  // want to stuff a string16 into some other API.
  std::wstring LossyDisplayName() const;

  // Return the path as ASCII, or the empty string if the path is not ASCII.
  // This should only be used for cases where the FilePath is representing a
  // known-ASCII filename.
  std::string MaybeAsASCII() const;

  // Return the path as UTF-8.
  //
  // This function is *unsafe* as there is no way to tell what encoding is
  // used in file names on POSIX systems other than Mac and Chrome OS,
  // although UTF-8 is practically used everywhere these days. To mitigate
  // the encoding issue, this function internally calls
  // SysNativeMBToWide() on POSIX systems other than Mac and Chrome OS,
  // per assumption that the current locale's encoding is used in file
  // names, but this isn't a perfect solution.
  //
  // Once it becomes safe to to stop caring about non-UTF-8 file names,
  // the SysNativeMBToWide() hack will be removed from the code, along
  // with "Unsafe" in the function name.
  std::string AsUTF8Unsafe() const;

  // Similar to AsUTF8Unsafe, but returns UTF-16 instead.
  std::wstring AsUTF16Unsafe() const;

  // Returns a FilePath object from a path name in UTF-8. This function
  // should only be used for cases where you are sure that the input
  // string is UTF-8.
  //
  // Like AsUTF8Unsafe(), this function is unsafe. This function
  // internally calls SysWideToNativeMB() on POSIX systems other than Mac
  // and Chrome OS, to mitigate the encoding issue. See the comment at
  // AsUTF8Unsafe() for details.
  static FilePath FromUTF8Unsafe(const std::string& utf8);

  // Similar to FromUTF8Unsafe, but accepts UTF-16 instead.
  static FilePath FromUTF16Unsafe(const std::wstring& utf16);

  // Normalize all path separators to backslash on Windows
  // (if FILE_PATH_USES_WIN_SEPARATORS is true), or do nothing on POSIX systems.
  FilePath NormalizePathSeparators() const;

  // Normalize all path separattors to given type on Windows
  // (if FILE_PATH_USES_WIN_SEPARATORS is true), or do nothing on POSIX systems.
  FilePath NormalizePathSeparatorsTo(CharType separator) const;

  // Compare two strings in the same way the file system does.
  // Note that these always ignore case, even on file systems that are case-
  // sensitive. If case-sensitive comparison is ever needed, add corresponding
  // methods here.
  // The methods are written as a static method so that they can also be used
  // on parts of a file path, e.g., just the extension.
  // CompareIgnoreCase() returns -1, 0 or 1 for less-than, equal-to and
  // greater-than respectively.
  static int CompareIgnoreCase(const StringType& string1,
                               const StringType& string2);
  static bool CompareEqualIgnoreCase(const StringType& string1,
                                     const StringType& string2) {
    return CompareIgnoreCase(string1, string2) == 0;
  }
  static bool CompareLessIgnoreCase(const StringType& string1,
                                    const StringType& string2) {
    return CompareIgnoreCase(string1, string2) < 0;
  }

  // bool IsExist() const { return is_exist_; }
  // bool IsFile() const { return is_exist_ && is_file_; }
  // bool IsDirectory() const { return is_exist_ && is_directory; };

#if defined(OS_ANDROID)
  // On android, file selection dialog can return a file with content uri
  // scheme(starting with content://). Content uri needs to be opened with
  // ContentResolver to guarantee that the app has appropriate permissions
  // to access it.
  // Returns true if the path is a content uri, or false otherwise.
  bool IsContentUri() const;
#endif

 private:
  // Remove trailing separators from this object.  If the path is absolute, it
  // will never be stripped any more than to refer to the absolute root
  // directory, so "////" will become "/", not "".  A leading pair of
  // separators is never stripped, to support alternate roots.  This is used to
  // support UNC paths on Windows.
  void StripTrailingSeparatorsInternal();

  bool is_exist_ = false;
  bool is_file_ = false;
  bool is_directory = false;

  StringType path_;
};

}  // namespace base

#endif  // SRC_BASE_FILE_FILE_PATH_H__