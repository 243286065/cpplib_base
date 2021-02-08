#include "base/file/file_path.h"

#include "base/file/file_path_constants.h"
#include "base/log/logging.h"
#include "base/string/string_convert.h"
#include "base/string/string_utils.h"
#include "base/string/stringprintf.h"

#if defined(OS_WIN)
#include <windows.h>
#include <algorithm>
#endif

namespace base {

using StringType = FilePath::StringType;

namespace {
const char* const kCommonDoubleExtensionSuffixes[] = {"gz", "xz", "bz2", "z",
                                                      "bz"};
const char* const kCommonDoubleExtensions[] = {"user.js"};

const FilePath::CharType kStringTerminator = FILE_PATH_LITERAL('\0');

StringType::size_type FindDriveLetter(const StringType& path) {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
  // This is dependent on an ASCII-based character set, but that's a
  // reasonable assumption.  iswalpha can be too inclusive here.
  if (path.length() >= 2 && path[1] == L':' &&
      ((path[0] >= L'A' && path[0] <= L'Z') ||
       (path[0] >= L'a' && path[0] <= L'z'))) {
    return 1;
  }
#endif  // FILE_PATH_USES_DRIVE_LETTERS
  return StringType::npos;
}

#if defined(FILE_PATH_USES_DRIVE_LETTERS)
bool EqualDriveLetterCaseInsensitive(const StringType& a, const StringType& b) {
  size_t a_letter_pos = FindDriveLetter(a);
  size_t b_letter_pos = FindDriveLetter(b);

  if (a_letter_pos == StringType::npos || b_letter_pos == StringType::npos)
    return a == b;

  StringType a_letter(a.substr(0, a_letter_pos + 1));
  StringType b_letter(b.substr(0, b_letter_pos + 1));
  if (!StartsWith(a_letter, b_letter, CompareCase::INSENSITIVE_ASCII))
    return false;

  StringType a_rest(a.substr(a_letter_pos + 1));
  StringType b_rest(b.substr(b_letter_pos + 1));
  return a_rest == b_rest;
}
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)

bool IsPathAbsolute(const StringType& path) {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
  StringType::size_type letter = FindDriveLetter(path);
  if (letter != StringType::npos) {
    // Look for a separator right after the drive specification.
    return path.length() > letter + 1 &&
           FilePath::IsSeparator(path[letter + 1]);
  }
  // Look for a pair of leading separators.
  return path.length() > 1 && FilePath::IsSeparator(path[0]) &&
         FilePath::IsSeparator(path[1]);
#else   // FILE_PATH_USES_DRIVE_LETTERS
  // Look for a separator in the first position.
  return path.length() > 0 && FilePath::IsSeparator(path[0]);
#endif  // FILE_PATH_USES_DRIVE_LETTERS
}

bool AreAllSeparators(const StringType& input) {
  for (auto it : input) {
    if (!FilePath::IsSeparator(it))
      return false;
  }

  return true;
}

// Find the position of the '.' that separates the extension from the rest
// of the file name. The position is relative to BaseName(), not value().
// Returns npos if it can't find an extension.
StringType::size_type FinalExtensionSeparatorPosition(const StringType& path) {
  // Special case "." and ".."
  if (path == FilePath::kCurrentDirectory || path == FilePath::kParentDirectory)
    return StringType::npos;

  return path.rfind(FilePath::kExtensionSeparator);
}

// Same as above, but allow a second extension component of up to 4
// characters when the rightmost extension component is a common double
// extension (gz, bz2, Z).  For example, foo.tar.gz or foo.tar.Z would have
// extension components of '.tar.gz' and '.tar.Z' respectively.
StringType::size_type ExtensionSeparatorPosition(const StringType& path) {
  const StringType::size_type last_dot = FinalExtensionSeparatorPosition(path);

  // No extension, or the extension is the whole filename.
  if (last_dot == StringType::npos || last_dot == 0U)
    return last_dot;

  const StringType::size_type penultimate_dot =
      path.rfind(FilePath::kExtensionSeparator, last_dot - 1);
  const StringType::size_type last_separator = path.find_last_of(
      FilePath::kSeparators, last_dot - 1, FilePath::kSeparatorsLength - 1);

  if (penultimate_dot == StringType::npos ||
      (last_separator != StringType::npos &&
       penultimate_dot < last_separator)) {
    return last_dot;
  }

  for (auto* i : kCommonDoubleExtensions) {
    StringType extension(path, penultimate_dot + 1);
    if (LowerCaseEqualsASCII(extension, i))
      return penultimate_dot;
  }

  StringType extension(path, last_dot + 1);
  for (auto* i : kCommonDoubleExtensionSuffixes) {
    if (LowerCaseEqualsASCII(extension, i)) {
      if ((last_dot - penultimate_dot) <= 5U &&
          (last_dot - penultimate_dot) > 1U) {
        return penultimate_dot;
      }
    }
  }

  return last_dot;
}

// Returns true if path is "", ".", or "..".
bool IsEmptyOrSpecialCase(const StringType& path) {
  // Special cases "", ".", and ".."
  if (path.empty() || path == FilePath::kCurrentDirectory ||
      path == FilePath::kParentDirectory) {
    return true;
  }

  return false;
}

}  // namespace

FilePath::FilePath() = default;

FilePath::FilePath(const FilePath& that) = default;
FilePath::FilePath(FilePath&& that) noexcept = default;

FilePath::FilePath(const StringType& path) : path_(path) {
  StringType::size_type nul_pos = path_.find(kStringTerminator);
  if (nul_pos != StringType::npos)
    path_.erase(nul_pos, StringType::npos);
}

FilePath::~FilePath() = default;

FilePath& FilePath::operator=(const FilePath& that) = default;

FilePath& FilePath::operator=(FilePath&& that) noexcept = default;

bool FilePath::operator==(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
  return EqualDriveLetterCaseInsensitive(this->path_, that.path_);
#else   // defined(FILE_PATH_USES_DRIVE_LETTERS)
  return path_ == that.path_;
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
}

bool FilePath::operator!=(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
  return !EqualDriveLetterCaseInsensitive(this->path_, that.path_);
#else   // defined(FILE_PATH_USES_DRIVE_LETTERS)
  return path_ != that.path_;
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
}

std::ostream& operator<<(std::ostream& out, const FilePath& file_path) {
  return out << file_path.value();
}

// static
bool FilePath::IsSeparator(CharType character) {
  for (size_t i = 0; i < kSeparatorsLength - 1; ++i) {
    if (character == kSeparators[i]) {
      return true;
    }
  }

  return false;
}

void FilePath::GetComponents(std::vector<StringType>* components) const {
  DCHECK(components);
  if (!components)
    return;
  components->clear();
  if (value().empty())
    return;

  std::vector<StringType> ret_val;
  FilePath current = *this;
  FilePath base;

  // Capture path components.
  while (current != current.DirName()) {
    base = current.BaseName();
    if (!AreAllSeparators(base.value()))
      ret_val.push_back(base.value());
    current = current.DirName();
  }

  // Capture root, if any.
  base = current.BaseName();
  if (!base.value().empty() && base.value() != kCurrentDirectory)
    ret_val.push_back(current.BaseName().value());

  // Capture drive letter, if any.
  FilePath dir = current.DirName();
  StringType::size_type letter = FindDriveLetter(dir.value());
  if (letter != StringType::npos) {
    ret_val.push_back(StringType(dir.value(), 0, letter + 1));
  }

  *components = std::vector<StringType>(ret_val.rbegin(), ret_val.rend());
}

bool FilePath::IsParent(const FilePath& child) const {
  return AppendRelativePath(child, nullptr);
}

bool FilePath::AppendRelativePath(const FilePath& child, FilePath* path) const {
  std::vector<StringType> parent_components;
  std::vector<StringType> child_components;
  GetComponents(&parent_components);
  child.GetComponents(&child_components);

  if (parent_components.empty() ||
      parent_components.size() >= child_components.size())
    return false;

  std::vector<StringType>::const_iterator parent_comp =
      parent_components.begin();
  std::vector<StringType>::const_iterator child_comp = child_components.begin();

#if defined(FILE_PATH_USES_DRIVE_LETTERS)
  // Windows can access case sensitive filesystems, so component
  // comparisions must be case sensitive, but drive letters are
  // never case sensitive.
  if ((FindDriveLetter(*parent_comp) != StringType::npos) &&
      (FindDriveLetter(*child_comp) != StringType::npos)) {
    if (!StartsWith(*parent_comp, *child_comp, CompareCase::INSENSITIVE_ASCII))
      return false;
    ++parent_comp;
    ++child_comp;
  }
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)

  while (parent_comp != parent_components.end()) {
    if (*parent_comp != *child_comp)
      return false;
    ++parent_comp;
    ++child_comp;
  }

  if (path != nullptr) {
    for (; child_comp != child_components.end(); ++child_comp) {
      *path = path->Append(*child_comp);
    }
  }
  return true;
}

// libgen's dirname and basename aren't guaranteed to be thread-safe and aren't
// guaranteed to not modify their input strings, and in fact are implemented
// differently in this regard on different platforms.  Don't use them, but
// adhere to their behavior.
FilePath FilePath::DirName() const {
  FilePath new_path(path_);
  new_path.StripTrailingSeparatorsInternal();

  // The drive letter, if any, always needs to remain in the output.  If there
  // is no drive letter, as will always be the case on platforms which do not
  // support drive letters, letter will be npos, or -1, so the comparisons and
  // resizes below using letter will still be valid.
  StringType::size_type letter = FindDriveLetter(new_path.path_);

  StringType::size_type last_separator = new_path.path_.find_last_of(
      kSeparators, StringType::npos, kSeparatorsLength - 1);
  if (last_separator == StringType::npos) {
    // path_ is in the current directory.
    new_path.path_.resize(letter + 1);
  } else if (last_separator == letter + 1) {
    // path_ is in the root directory.
    new_path.path_.resize(letter + 2);
  } else if (last_separator == letter + 2 &&
             IsSeparator(new_path.path_[letter + 1])) {
    // path_ is in "//" (possibly with a drive letter); leave the double
    // separator intact indicating alternate root.
    new_path.path_.resize(letter + 3);
  } else if (last_separator != 0) {
    // path_ is somewhere else, trim the basename.
    new_path.path_.resize(last_separator);
  }

  new_path.StripTrailingSeparatorsInternal();
  if (!new_path.path_.length())
    new_path.path_ = kCurrentDirectory;

  return new_path;
}

FilePath FilePath::BaseName() const {
  FilePath new_path(path_);
  new_path.StripTrailingSeparatorsInternal();

  // The drive letter, if any, is always stripped.
  StringType::size_type letter = FindDriveLetter(new_path.path_);
  if (letter != StringType::npos) {
    new_path.path_.erase(0, letter + 1);
  }

  // Keep everything after the final separator, but if the pathname is only
  // one character and it's a separator, leave it alone.
  StringType::size_type last_separator = new_path.path_.find_last_of(
      kSeparators, StringType::npos, kSeparatorsLength - 1);
  if (last_separator != StringType::npos &&
      last_separator < new_path.path_.length() - 1) {
    new_path.path_.erase(0, last_separator + 1);
  }

  return new_path;
}

StringType FilePath::Extension() const {
  FilePath base(BaseName());
  const StringType::size_type dot = ExtensionSeparatorPosition(base.path_);
  if (dot == StringType::npos)
    return StringType();

  return base.path_.substr(dot, StringType::npos);
}

StringType FilePath::FinalExtension() const {
  FilePath base(BaseName());
  const StringType::size_type dot = FinalExtensionSeparatorPosition(base.path_);
  if (dot == StringType::npos)
    return StringType();

  return base.path_.substr(dot, StringType::npos);
}

FilePath FilePath::RemoveExtension() const {
  if (Extension().empty())
    return *this;

  const StringType::size_type dot = ExtensionSeparatorPosition(path_);
  if (dot == StringType::npos)
    return *this;

  return FilePath(path_.substr(0, dot));
}

FilePath FilePath::RemoveFinalExtension() const {
  if (FinalExtension().empty())
    return *this;

  const StringType::size_type dot = FinalExtensionSeparatorPosition(path_);
  if (dot == StringType::npos)
    return *this;

  return FilePath(path_.substr(0, dot));
}

FilePath FilePath::InsertBeforeExtension(const StringType& suffix) const {
  if (suffix.empty())
    return FilePath(path_);

  if (IsEmptyOrSpecialCase(BaseName().value()))
    return FilePath();

  StringType new_path = RemoveExtension().value() + suffix + Extension();
  return FilePath(new_path);
}

FilePath FilePath::AddExtension(const StringType& extension) const {
  if (IsEmptyOrSpecialCase(BaseName().value()))
    return FilePath();

  // If the new extension is "" or ".", then just return the current FilePath.
  if (extension.empty() ||
      (extension.size() == 1 && extension[0] == kExtensionSeparator))
    return *this;

  StringType str = path_;
  if (extension[0] != kExtensionSeparator &&
      *(str.end() - 1) != kExtensionSeparator) {
    str.append(1, kExtensionSeparator);
  }
  str.append(extension.data(), extension.size());
  return FilePath(str);
}

FilePath FilePath::ReplaceExtension(const StringType& extension) const {
  if (IsEmptyOrSpecialCase(BaseName().value()))
    return FilePath();

  FilePath no_ext = RemoveExtension();
  // If the new extension is "" or ".", then just remove the current extension.
  if (extension.empty() ||
      (extension.size() == 1 && extension[0] == kExtensionSeparator))
    return no_ext;

  StringType str = no_ext.value();
  if (extension[0] != kExtensionSeparator)
    str.append(1, kExtensionSeparator);
  str.append(extension.data(), extension.size());
  return FilePath(str);
}

bool FilePath::MatchesExtension(const StringType& extension) const {
  DCHECK(extension.empty() || extension[0] == kExtensionSeparator);

  StringType current_extension = Extension();

  if (current_extension.length() != extension.length())
    return false;

  return FilePath::CompareEqualIgnoreCase(extension, current_extension);
}

FilePath FilePath::Append(const StringType& component) const {
  StringType appended = component;
  StringType without_nuls;

  StringType::size_type nul_pos = component.find(kStringTerminator);
  if (nul_pos != StringType::npos) {
    without_nuls = StringType(component.substr(0, nul_pos));
    appended = StringType(without_nuls);
  }

  DCHECK(!IsPathAbsolute(appended));

  if (path_.compare(kCurrentDirectory) == 0 && !appended.empty()) {
    // Append normally doesn't do any normalization, but as a special case,
    // when appending to kCurrentDirectory, just return a new path for the
    // component argument.  Appending component to kCurrentDirectory would
    // serve no purpose other than needlessly lengthening the path, and
    // it's likely in practice to wind up with FilePath objects containing
    // only kCurrentDirectory when calling DirName on a single relative path
    // component.
    return FilePath(appended);
  }

  FilePath new_path(path_);
  new_path.StripTrailingSeparatorsInternal();

  // Don't append a separator if the path is empty (indicating the current
  // directory) or if the path component is empty (indicating nothing to
  // append).
  if (!appended.empty() && !new_path.path_.empty()) {
    // Don't append a separator if the path still ends with a trailing
    // separator after stripping (indicating the root directory).
    if (!IsSeparator(new_path.path_.back())) {
      // Don't append a separator if the path is just a drive letter.
      if (FindDriveLetter(new_path.path_) + 1 != new_path.path_.length()) {
        new_path.path_.append(1, kSeparators[0]);
      }
    }
  }

  new_path.path_.append(appended.data(), appended.size());
  return new_path;
}

FilePath FilePath::Append(const FilePath& component) const {
  return Append(component.value());
}

bool FilePath::IsAbsolute() const {
  return IsPathAbsolute(path_);
}

bool FilePath::EndsWithSeparator() const {
  if (empty())
    return false;
  return IsSeparator(path_.back());
}

FilePath FilePath::AsEndingWithSeparator() const {
  if (EndsWithSeparator() || path_.empty())
    return *this;

  StringType path_str;
  path_str.reserve(path_.length() + 1);  // Only allocate string once.

  path_str = path_;
  path_str.append(&kSeparators[0], 1);
  return FilePath(path_str);
}

FilePath FilePath::StripTrailingSeparators() const {
  FilePath new_path(path_);
  new_path.StripTrailingSeparatorsInternal();

  return new_path;
}

bool FilePath::ReferencesParent() const {
  if (path_.find(kParentDirectory) == StringType::npos) {
    // GetComponents is quite expensive, so avoid calling it in the majority
    // of cases where there isn't a kParentDirectory anywhere in the path.
    return false;
  }

  std::vector<StringType> components;
  GetComponents(&components);

  std::vector<StringType>::const_iterator it = components.begin();
  for (; it != components.end(); ++it) {
    const StringType& component = *it;
    // Windows has odd, undocumented behavior with path components containing
    // only whitespace and . characters. So, if all we see is . and
    // whitespace, then we treat any .. sequence as referencing parent.
    // For simplicity we enforce this on all platforms.
    if (component.find_first_not_of(FILE_PATH_LITERAL(". \n\r\t")) ==
            std::string::npos &&
        component.find(kParentDirectory) != std::string::npos) {
      return true;
    }
  }
  return false;
}

#if defined(OS_WIN)

std::wstring FilePath::LossyDisplayName() const {
  return path_;
}

std::string FilePath::MaybeAsASCII() const {
  return base::UnicodeToAnsi(path_);
}

std::string FilePath::AsUTF8Unsafe() const {
  return base::UnicodeToAnsi(path_);
}

std::wstring FilePath::AsUTF16Unsafe() const {
  return path_;
}

// static
FilePath FilePath::FromUTF8Unsafe(const std::string& utf8) {
  return FilePath(AnsiToUnicode(utf8));
}

// static
FilePath FilePath::FromUTF16Unsafe(const std::wstring& utf16) {
  return FilePath(utf16);
}

#elif defined(OS_LINUX)
// See file_path.h for a discussion of the encoding of paths on POSIX
// platforms.  These encoding conversion functions are not quite correct.

std::wstring FilePath::LossyDisplayName() const {
  return AnsiToUnicode(path_);
}

std::string FilePath::MaybeAsASCII() const {
  return path_;
}

std::string FilePath::AsUTF8Unsafe() const {
  return path_;
}

std::wstring FilePath::AsUTF16Unsafe() const {
  return AnsiToUnicode(path_);
}

// static
FilePath FilePath::FromUTF8Unsafe(const std::string& utf8) {
  return FilePath(utf8);
}

// static
FilePath FilePath::FromUTF16Unsafe(const std::wstring& utf16) {
  return FilePath(UnicodeToAnsi(utf16));
}

#endif  // defined(OS_WIN)

#if defined(OS_WIN)
// Windows specific implementation of file string comparisons.

int FilePath::CompareIgnoreCase(const StringType& string1,
                                const StringType& string2) {
  // CharUpperW within user32 is used here because it will provide unicode
  // conversions regardless of locale. The STL alternative, towupper, has a
  // locale consideration that prevents it from converting all characters by
  // default.
  // TODO: CHECK(win::IsUser32AndGdi32Available());
  // Perform character-wise upper case comparison rather than using the
  // fully Unicode-aware CompareString(). For details see:
  // http://blogs.msdn.com/michkap/archive/2005/10/17/481600.aspx
  StringType::const_iterator i1 = string1.begin();
  StringType::const_iterator i2 = string2.begin();
  StringType::const_iterator string1end = string1.end();
  StringType::const_iterator string2end = string2.end();
  for (; i1 != string1end && i2 != string2end; ++i1, ++i2) {
    wchar_t c1 =
        (wchar_t)LOWORD(::CharUpperW((LPWSTR)(DWORD_PTR)MAKELONG(*i1, 0)));
    wchar_t c2 =
        (wchar_t)LOWORD(::CharUpperW((LPWSTR)(DWORD_PTR)MAKELONG(*i2, 0)));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
  }
  if (i1 != string1end)
    return 1;
  if (i2 != string2end)
    return -1;
  return 0;
}
#elif defined(OS_LINUX)
// Generic Posix system comparisons.
int FilePath::CompareIgnoreCase(const StringType& string1,
                                const StringType& string2) {
  // Specifically need null termianted strings for this API call.
  int comparison =
      strcasecmp(string1.c_str(), string2.c_str());
  if (comparison < 0)
    return -1;
  if (comparison > 0)
    return 1;
  return 0;
}
#endif

void FilePath::StripTrailingSeparatorsInternal() {
  // If there is no drive letter, start will be 1, which will prevent stripping
  // the leading separator if there is only one separator.  If there is a drive
  // letter, start will be set appropriately to prevent stripping the first
  // separator following the drive letter, if a separator immediately follows
  // the drive letter.
  StringType::size_type start = FindDriveLetter(path_) + 2;

  StringType::size_type last_stripped = StringType::npos;
  for (StringType::size_type pos = path_.length();
       pos > start && IsSeparator(path_[pos - 1]); --pos) {
    // If the string only has two separators and they're at the beginning,
    // don't strip them, unless the string began with more than two separators.
    if (pos != start + 1 || last_stripped == start + 2 ||
        !IsSeparator(path_[start - 1])) {
      path_.resize(pos - 1);
      last_stripped = pos;
    }
  }
}

FilePath FilePath::NormalizePathSeparators() const {
  return NormalizePathSeparatorsTo(kSeparators[0]);
}

FilePath FilePath::NormalizePathSeparatorsTo(CharType separator) const {
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
  DCHECK_NE(kSeparators + kSeparatorsLength,
            std::find(kSeparators, kSeparators + kSeparatorsLength, separator));
  StringType copy = path_;
  for (size_t i = 0; i < kSeparatorsLength; ++i) {
    std::replace(copy.begin(), copy.end(), kSeparators[i], separator);
  }
  return FilePath(copy);
#else
  return *this;
#endif
}

#if defined(OS_ANDROID)
bool FilePath::IsContentUri() const {
  return StartsWith(path_, "content://", base::CompareCase::INSENSITIVE_ASCII);
}
#endif
}  // namespace base
