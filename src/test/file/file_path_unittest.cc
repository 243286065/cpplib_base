#include "base/log/logging.h"
#include "base/file/file_path.h"
#include "gtest/gtest.h"

namespace base
{

#if defined(OS_LINUX)
TEST(FilePathTest, NormalFunctionLinux) {
  FilePath path("/tmp//test.txt");
  FilePath new_path(path);
  EXPECT_EQ(new_path.BaseName().value(), "test.txt");
  EXPECT_EQ(new_path.DirName().value(), "/tmp");
  EXPECT_EQ(new_path.NormalizePathSeparators().value(), "/tmp//test.txt");
  EXPECT_FALSE(new_path.empty());
  EXPECT_EQ(new_path.Extension(), ".txt");
  EXPECT_EQ(new_path.FinalExtension(), ".txt");

  std::vector<std::string> component;
  new_path.GetComponents(&component);
  EXPECT_EQ(component.size(), 3);
  EXPECT_EQ(component[0], "/");
  EXPECT_EQ(component[1], "tmp");
  EXPECT_EQ(component[2], "test.txt");

  FilePath child("/tmp/test.txt/test");
  EXPECT_TRUE(new_path.IsParent(child));
  FilePath child2("/tmp/test.txt2/test");
  EXPECT_FALSE(new_path.IsParent(child2));

  FilePath parent3("/Users/johndoe/Library/Application Support");
  FilePath child3("/Users/johndoe/Library/Application Support/Google/Chrome/Default");
  FilePath child4("/Users/johndoe/Library/Caches");
  bool rs = parent3.AppendRelativePath(child3, &child4);
  EXPECT_TRUE(rs);
  EXPECT_EQ(parent3.value(), "/Users/johndoe/Library/Application Support");
  EXPECT_EQ(child3.value(), "/Users/johndoe/Library/Application Support/Google/Chrome/Default");
  EXPECT_EQ(child4.value(), "/Users/johndoe/Library/Caches/Google/Chrome/Default");
  EXPECT_FALSE(parent3.AppendRelativePath(FilePath("/Users/johndoe/Library/Application Support1/Google/Chrome/Default"), &child4));

  FilePath path5("/var/log/log.tar.gz");
  EXPECT_EQ(path5.Extension(), ".tar.gz");
  EXPECT_EQ(path5.FinalExtension(), ".gz");
  EXPECT_EQ(path5.RemoveFinalExtension().value(), "/var/log/log.tar");
  EXPECT_EQ(path5.RemoveExtension().value(), "/var/log/log");
  EXPECT_EQ(path5.AddExtension(".z").value(), "/var/log/log.tar.gz.z");
  EXPECT_EQ(path5.ReplaceExtension(".z").value(), "/var/log/log.z");
  EXPECT_TRUE(path5.MatchesExtension(".tar.gz"));
  EXPECT_FALSE(path5.MatchesExtension(".gz"));
  
  EXPECT_TRUE(path5.IsAbsolute());
  EXPECT_FALSE(FilePath("../test").IsAbsolute());
  EXPECT_FALSE(path5.EndsWithSeparator());
  EXPECT_TRUE(FilePath("/var/log/").EndsWithSeparator());
  LOG(INFO) << path5.AsEndingWithSeparator().value();
  EXPECT_TRUE(path5.AsEndingWithSeparator().EndsWithSeparator());
  LOG(INFO) << path5.AsEndingWithSeparator().StripTrailingSeparators().value();
  EXPECT_FALSE(path5.AsEndingWithSeparator().StripTrailingSeparators().EndsWithSeparator());
  EXPECT_TRUE(FilePath("../test").ReferencesParent());
  EXPECT_FALSE(path5.ReferencesParent());

  FilePath path6("test1/test2");
  path6 = path6.Append("test3");
  EXPECT_EQ(path6.value(), "test1/test2/test3");
  FilePath path7("test4/test5");
  path6 = path6.Append(path7);
  EXPECT_EQ(path6.value(), "test1/test2/test3/test4/test5");
}

TEST(FilePathTest, StaticFunctionLinux) {
  EXPECT_TRUE(FilePath::IsSeparator('/'));
  EXPECT_FALSE(FilePath::IsSeparator('\\'));

  std::string name1 = "test/test3";
  std::wstring name2 = L"test/test3";
  FilePath path1 = FilePath::FromUTF8Unsafe(name1);
  FilePath path2 = FilePath::FromUTF16Unsafe(name2);
  EXPECT_EQ(path1.value(), path2.value());

  EXPECT_EQ(FilePath::CompareIgnoreCase("123456", "123456"), 0);
  EXPECT_EQ(FilePath::CompareIgnoreCase("223456", "123456"), 1);
  EXPECT_EQ(FilePath::CompareIgnoreCase("123456", "223456"), -1);
  EXPECT_EQ(FilePath::CompareIgnoreCase("123456", "1134567"), 1);
}
#endif

} // namespace base
