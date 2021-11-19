#include "common.hpp"
#include <gtest/gtest.h>

TEST(func, ToLower) {
    EXPECT_EQ("abc", ToLower("ABC"));
    EXPECT_EQ("123abc", ToLower("123aBC"));
}

TEST(func, FormatToString) {
    EXPECT_EQ("abc", FormatToString("%s", "abc"));
    EXPECT_EQ("abc+123", FormatToString("%s+%d", "abc", 123));
    EXPECT_EQ("abc+123+123.123", FormatToString("%s+%d+%.3f", "abc", 123, 123.123));
    EXPECT_EQ("this is a string", FormatToString("this is a string"));
    const char *pchar = "abc";
    EXPECT_EQ("this is abc", FormatToString("this is %s", pchar));
}

TEST(func,IntToCStr){
	const char *c_str="123";
	EXPECT_STREQ(c_str,IntToCStr(123));
}

TEST(func,GetFileExtension){
	std::string filename = "gtest.html";
	EXPECT_EQ("html",GetFileExtension(filename));
	EXPECT_EQ("php",GetFileExtension("gtest.php"));
}
