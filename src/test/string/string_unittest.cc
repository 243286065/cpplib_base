#include "base/log/logging.h"
#include "base/string/string_convert.h"
#include "base/string/string_utils.h"
#include "gtest/gtest.h"

namespace base {

TEST(StringTest, IsHexDigit) {
  EXPECT_TRUE(IsHexDigit('0'));
  EXPECT_TRUE(IsHexDigit('9'));
  EXPECT_TRUE(IsHexDigit('a'));
  EXPECT_TRUE(IsHexDigit('A'));
  EXPECT_TRUE(IsHexDigit('f'));

  EXPECT_FALSE(IsHexDigit('+'));
  EXPECT_FALSE(IsHexDigit('h'));
}

TEST(StringTest, ToLowerASCII) {
  EXPECT_EQ(ToLowerASCII('A'), 'a');
  EXPECT_EQ(ToLowerASCII('a'), 'a');
  EXPECT_EQ(ToLowerASCII('1'), '1');
  EXPECT_NE(ToLowerASCII('A'), 'b');
  EXPECT_EQ(ToLowerASCII(L'A'), L'a');
  EXPECT_EQ(ToLowerASCII(L'a'), L'a');

  EXPECT_EQ(ToLowerASCII(""), "");
  EXPECT_EQ(ToLowerASCII("ABcdefg123Gh"), "abcdefg123gh");
  EXPECT_EQ(ToLowerASCII(L"ABcdefg123Gh"), L"abcdefg123gh");
}

TEST(StringTest, ToUpperASCII) {
  EXPECT_EQ(ToUpperASCII('A'), 'A');
  EXPECT_EQ(ToUpperASCII('a'), 'A');
  EXPECT_EQ(ToUpperASCII('1'), '1');
  EXPECT_NE(ToUpperASCII('A'), 'B');
  EXPECT_EQ(ToUpperASCII(L'A'), L'A');
  EXPECT_EQ(ToUpperASCII(L'a'), L'A');

  EXPECT_EQ(ToUpperASCII(""), "");
  EXPECT_EQ(ToUpperASCII("ABcdefg_*123Gh"), "ABCDEFG_*123GH");
  EXPECT_EQ(ToUpperASCII(L"ABcdefg_*123Gh"), L"ABCDEFG_*123GH");
  EXPECT_EQ(ToUpperASCII(L"中国人a"), L"中国人A");
}

TEST(StringTest, StartsWith) {
  EXPECT_TRUE(StartsWith("123A34dsad_dsad@32131", "123A34dsad"));
  EXPECT_TRUE(StartsWith("123A34dsad_dsad@32131", "123"));
  EXPECT_TRUE(StartsWith(L"123A34dsad_dsad@32131", L"123A34dsad"));
  EXPECT_TRUE(StartsWith(L"123A34dsad_dsad@32131", L"123"));
  EXPECT_TRUE(StartsWith(L"123中国人~", L"123中"));

  EXPECT_FALSE(StartsWith(L"123中国人~", L"1123中"));
  EXPECT_FALSE(StartsWith("123A34dsad_dsad@32131", " 123A34dsad"));

  EXPECT_TRUE(StartsWith("123A34dsad_dsad@32131", "123a34Dsad",
                         base::CompareCase::INSENSITIVE_ASCII));
  EXPECT_FALSE(StartsWith("123A34dsad_dsad@32131", "123a34dsad",
                          base::CompareCase::SENSITIVE));
  EXPECT_TRUE(StartsWith(L"123A34dsad_dsad@32131", L"123a34Dsad",
                         base::CompareCase::INSENSITIVE_ASCII));
  EXPECT_FALSE(StartsWith(L"123A34dsad_dsad@32131", L"123a34dsad",
                          base::CompareCase::SENSITIVE));
}

TEST(StringTest, EndsWith) {
  EXPECT_TRUE(EndsWith("123A34dsad_dsad@32131", "123A34dsad_dsad@32131"));
  EXPECT_TRUE(EndsWith("123A34dsad_dsad@32131", "_dsad@32131"));
  EXPECT_TRUE(EndsWith(L"123A34dsad_dsad@32131", L"_dsad@32131"));
  EXPECT_TRUE(EndsWith(L"123A34dsad_dsad@32131", L"d@32131"));
  EXPECT_TRUE(EndsWith(L"123中国人~", L"人~"));

  EXPECT_FALSE(EndsWith(L"123中国人~", L"中国人~1"));
  EXPECT_FALSE(EndsWith("123A34dsad_dsad@32131", " @32131"));

  EXPECT_TRUE(EndsWith("123A34dsad_dsad@32131", "_DsaD@32131",
                       base::CompareCase::INSENSITIVE_ASCII));
  EXPECT_FALSE(EndsWith("123A34dsad_dsad@32131", "_DsaD@32131",
                        base::CompareCase::SENSITIVE));
  EXPECT_TRUE(EndsWith(L"123A34dsad_dsad@32131", L"_DsaD@32131",
                       base::CompareCase::INSENSITIVE_ASCII));
  EXPECT_FALSE(EndsWith(L"123A34dsad_dsad@32131", L"_DsaD@32131",
                        base::CompareCase::SENSITIVE));
}

TEST(StringTest, CompareCaseInsensitiveASCII) {
  EXPECT_EQ(CompareCaseInsensitiveASCII("abcTest", "abcTest"), 0);
  EXPECT_EQ(CompareCaseInsensitiveASCII("bbcTest", "abcTest"), 1);
  EXPECT_EQ(CompareCaseInsensitiveASCII("aacTest", "abcTest"), -1);
  EXPECT_EQ(CompareCaseInsensitiveASCII("abcTest", "aacTest123"), 1);
  EXPECT_EQ(CompareCaseInsensitiveASCII("abcTest", "abcTest123"), -1);
  EXPECT_EQ(CompareCaseInsensitiveASCII("abcTest", "aBcTest"), 0);

  EXPECT_EQ(CompareCaseInsensitiveASCII(L"abcTest", L"abcTest"), 0);
  EXPECT_EQ(CompareCaseInsensitiveASCII(L"bbcTest", L"abcTest"), 1);
  EXPECT_EQ(CompareCaseInsensitiveASCII(L"aacTest", L"abcTest"), -1);
  EXPECT_EQ(CompareCaseInsensitiveASCII(L"abcTest", L"aacTest123"), 1);
  EXPECT_EQ(CompareCaseInsensitiveASCII(L"abcTest", L"abcTest123"), -1);
  EXPECT_EQ(CompareCaseInsensitiveASCII(L"abcTest", L"aBCTest"), 0);
}

TEST(StringTest, EqualsCaseInsensitiveASCII) {
  EXPECT_TRUE(EqualsCaseInsensitiveASCII("abcTest", "abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("bbcTest", "abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("aacTest", "abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("abcTest", "aacTest123"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("abcTest", "abcTest123"));
  EXPECT_TRUE(EqualsCaseInsensitiveASCII("abcTest", "aBcTest"));

  EXPECT_TRUE(EqualsCaseInsensitiveASCII(L"abcTest", L"abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII(L"bbcTest", L"abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII(L"aacTest", L"abcTest"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII(L"abcTest", L"aacTest123"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII(L"abcTest", L"abcTest123"));
  EXPECT_TRUE(EqualsCaseInsensitiveASCII(L"abcTest", L"aBCTest"));
}

TEST(StringTest, IsDigitStrs) {
  EXPECT_TRUE(IsDigitStrs("1234566732423424323432542"));
  EXPECT_FALSE(IsDigitStrs("-1234566732423424323432542"));

  EXPECT_TRUE(IsDigitStrs(L"1234566732423424323432542"));
  EXPECT_FALSE(IsDigitStrs(L"-1234566732423424323432542"));
}

TEST(StringTest, SplitStr) {
  std::string str = "abc=bcd=cdf=123:789=8765:9087";
  auto rs = SplitStr(str, '=');
  EXPECT_EQ(rs.size(), 5);
  EXPECT_EQ(rs[0], "abc");
  EXPECT_EQ(rs[1], "bcd");
  EXPECT_EQ(rs[2], "cdf");
  EXPECT_EQ(rs[3], "123:789");
  EXPECT_EQ(rs[4], "8765:9087");

  auto rs2 = SplitStr(str, ':');
  EXPECT_EQ(rs2.size(), 3);
  EXPECT_EQ(rs2[0], "abc=bcd=cdf=123");
  EXPECT_EQ(rs2[1], "789=8765");
  EXPECT_EQ(rs2[2], "9087");

  auto rs3 = SplitStr(":123:", ':');
  EXPECT_EQ(rs3.size(), 2);
  EXPECT_EQ(rs3[0], "");
  EXPECT_EQ(rs3[1], "123");

  auto rs4 = SplitStr("::", ':');
  EXPECT_EQ(rs4.size(), 2);
  EXPECT_EQ(rs4[0], "");
  EXPECT_EQ(rs4[1], "");
}

TEST(StringTest, AnsiToUnicode) {
  EXPECT_EQ(AnsiToUnicode("abctest"), L"abctest");
  EXPECT_EQ(AnsiToUnicode("BYussfdasfdsfcdsfs"), L"BYussfdasfdsfcdsfs");
  EXPECT_EQ(AnsiToUnicode(UnicodeToAnsi(L"中国人demo")), L"中国人demo");
}

TEST(StringTest, UnicodeToAnsi) {
  EXPECT_EQ(UnicodeToAnsi(L"abctest"), "abctest");
  EXPECT_EQ(UnicodeToAnsi(L"BYussfdasfdsfcdsfs"), "BYussfdasfdsfcdsfs");
  EXPECT_EQ(UnicodeToAnsi(AnsiToUnicode("ABCDF*")), "ABCDF*");
}

TEST(StringTest, LowerCaseEqualsASCII) {
	EXPECT_TRUE(LowerCaseEqualsASCII("Abcdefghigk", "abcdefghigk"));
	EXPECT_FALSE(LowerCaseEqualsASCII("Abcdefghigkl", "abcdefghigk"));

	EXPECT_TRUE(LowerCaseEqualsASCII(L"Abcdefghigk", "abcdefghigk"));
	EXPECT_FALSE(LowerCaseEqualsASCII(L"Abcdefghigkl", "abcdefghigk"));
}

}  // namespace base