#include "../x.h"
#include <gtest/gtest.h>

TEST(StringOpsTest, SplitCharDelimiter) {
    auto parts = x::split("hello,world,cpp", ",");
    EXPECT_EQ(parts.size(), 3);
    EXPECT_EQ(parts[0], "hello");
    EXPECT_EQ(parts[1], "world");
    EXPECT_EQ(parts[2], "cpp");
}

TEST(StringOpsTest, SplitStringDelimiter) {
    auto parts = x::split("hello::world::cpp", "::");
    EXPECT_EQ(parts.size(), 3);
    EXPECT_EQ(parts[0], "hello");
    EXPECT_EQ(parts[1], "world");
    EXPECT_EQ(parts[2], "cpp");
}

TEST(StringOpsTest, JoinStrings) {
    std::vector<std::string> parts = {"hello", "world", "cpp"};
    auto joined = x::join(parts, ", ");
    EXPECT_EQ(joined, "hello, world, cpp");
}

TEST(StringOpsTest, EdgeCases) {
    auto empty = x::split("", ",");
    EXPECT_TRUE(empty.empty());
    
    auto no_delim = x::split("hello", ",");
    EXPECT_EQ(no_delim.size(), 0);
    
    std::vector<std::string> empty_parts;
    EXPECT_EQ(x::join(empty_parts, ","), "");
}

TEST(StringOpsTest, Trim) {
    EXPECT_EQ(x::trim("  hello  "), "hello");
    EXPECT_EQ(x::trim("\thello\t"), "hello");
    EXPECT_EQ(x::trim("  hello world  "), "hello world");
    EXPECT_EQ(x::trim(""), "");
}

TEST(StringOpsTest, Replace) {
    EXPECT_EQ(x::replace("hello world", "world", "cpp"), "hello cpp");
    EXPECT_EQ(x::replace("banana", "na", "no"), "banono");
    EXPECT_EQ(x::replace("hello", "x", "y"), "hello");
    EXPECT_EQ(x::replace("", "x", "y"), "");
}

TEST(StringOpsTest, Reverse) {
    EXPECT_EQ(x::reverse("hello"), "olleh");
    EXPECT_EQ(x::reverse(""), "");
    EXPECT_EQ(x::reverse("a"), "a");
    EXPECT_EQ(x::reverse("ab"), "ba");
}

TEST(StringOpsTest, UpperLower) {
    EXPECT_EQ(x::upper("hello"), "HELLO");
    EXPECT_EQ(x::lower("HELLO"), "hello");
    EXPECT_EQ(x::upper(""), "");
    EXPECT_EQ(x::lower(""), "");
    EXPECT_EQ(x::upper("123"), "123");
    EXPECT_EQ(x::lower("123"), "123");
}

TEST(StringOpsTest, StartEndWith) {
    EXPECT_TRUE(x::startWith("hello world", "hello"));
    EXPECT_FALSE(x::startWith("hello world", "world"));
    EXPECT_TRUE(x::endWith("hello world", "world"));
    EXPECT_FALSE(x::endWith("hello world", "hello"));
    EXPECT_FALSE(x::startWith("", "x"));
    EXPECT_FALSE(x::endWith("", "x"));
}

TEST(StringOpsTest, Contain) {
    EXPECT_TRUE(x::contain("hello world", "llo"));
    EXPECT_FALSE(x::contain("hello world", "cpp"));
    EXPECT_FALSE(x::contain("", "x"));
    EXPECT_TRUE(x::contain("x", "x"));
}

TEST(StringOpsTest, IsDigit) {
    EXPECT_TRUE(x::is_digit("12345"));
    EXPECT_FALSE(x::is_digit("123a45"));
    EXPECT_FALSE(x::is_digit(""));
    EXPECT_FALSE(x::is_digit(" "));
    EXPECT_TRUE(x::is_digit("1.23"));
    EXPECT_TRUE(x::is_digit("-123"));
    EXPECT_TRUE(x::is_digit("0"));
    EXPECT_FALSE(x::is_digit("١٢٣"));  // Arabic numerals
}

TEST(StringOpsTest, SameNoCase) {
    EXPECT_TRUE(x::same_nocase("hello", "HELLO"));
    EXPECT_TRUE(x::same_nocase("HELLO", "hello"));
    EXPECT_TRUE(x::same_nocase("hElLo", "HeLlO"));
    EXPECT_FALSE(x::same_nocase("hello", "world"));
    EXPECT_FALSE(x::same_nocase("hello", "hell"));
    EXPECT_TRUE(x::same_nocase("", ""));
    EXPECT_FALSE(x::same_nocase("", "x"));
    EXPECT_FALSE(x::same_nocase("x", ""));
}

TEST(StringOpsTest, VariadicFormat) {
    EXPECT_EQ(x::vfmt("{} {}", 1, 2), "1 2");
    EXPECT_EQ(x::vfmt("Hello {}", "world"), "Hello world");
    EXPECT_EQ(x::vfmt(""), "");
}

TEST(StringOpsTest, VariadicPrint) {
    testing::internal::CaptureStdout();
    x::vprt("Hello {}", "world");
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "Hello world");

    testing::internal::CaptureStdout();
    x::vprtln("Hello {}", "world");
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "Hello world\n");

    testing::internal::CaptureStderr();
    x::veprt("Error: {}", "message");
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "Error: message");

    testing::internal::CaptureStderr();
    x::veprtln("Error: {}", "message");
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "Error: message\n");
}
