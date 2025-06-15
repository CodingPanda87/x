#include "../x.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

using namespace x;

namespace {
    class PrintTest : public ::testing::Test {
    protected:
        std::stringstream buffer;
        std::streambuf* old_cout;

        void SetUp() override {
            old_cout = std::cout.rdbuf(buffer.rdbuf());
        }

        void TearDown() override {
            std::cout.rdbuf(old_cout);
        }
    };
}

TEST_F(PrintTest, PrintFunction) {
    std::print("Test {} {}", 1, 2);
    EXPECT_EQ(buffer.str(), "Test 1 2");
    buffer.str("");
    
    std::println("Hello {}", "world");
    EXPECT_EQ(buffer.str(), "Hello world\n");
    buffer.str("");
}

TEST(XTest, BasicTest) {
    _vec<u32> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    cU16 n = 1;
    // n = 16; // error
    _coto c = 1;
    // c = 16; // error
    EXPECT_EQ(sizeof(c),4);
    void *p = _nul;
    EXPECT_EQ(p, _nul);
    auto s = _s("Hello");
    EXPECT_EQ(s, "Hello");
    auto ss = _fmt("{},world!", s);
    EXPECT_EQ(ss, "Hello,world!");
}

TEST(XTest,Print){
    _prt("Hello {},", "x-lib");
    _prtln("Hello {}", "world");
    _prtln("Hello {}", "world");
    str s = "hello {} ";
    EXPECT_EQ(vfmt(s, "world"), "hello world ");
    vprt(s, "x-lib");
    vprtln(s, "x-lib");
    
    // Test empty format string
    EXPECT_EQ(vfmt("", "ignored"), "");
    
    // Test multiple arguments
    EXPECT_EQ(vfmt("{} {} {}", 1, 2, 3), "1 2 3");
    
    // Test special characters
    EXPECT_EQ(vfmt("\\t\\n\\r"), "\\t\\n\\r");
    
    // Test format string with only literals
    EXPECT_EQ(vfmt("literal string"), "literal string");
}

TEST(XTest, PrintEdgeCases) {
    // Test printing empty string
    testing::internal::CaptureStdout();
    _prt("");
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
    
    // Test printing special characters
    testing::internal::CaptureStdout();
    _prt("\t\n\r");
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\t\n\r");
}

TEST(XTest, CompileTimeConcat) {
    #define S1  "hi,"
    #define S2  "x-lib"
    constexpr auto s1 = _cat("hello", "world");
    static_assert(s1 == "helloworld", "Concat failed");
    constexpr auto s2 = _cat(S1, S2);
    static_assert(s2 == "hi,x-lib", "Concat failed");
    constexpr auto s3 = _cat(S1, "x-lib");
    static_assert(s3 == "hi,x-lib", "Concat failed");
}

TEST(XTest, MacroTest) {
    EXPECT_EQ(_code_info(),_fmt("test_basic.cpp/{}/{}", __LINE__,__func__));
    EXPECT_EQ(_excode_info("123"), _fmt("test_basic.cpp/{}/123", __LINE__));
    str build_time = _build_time();
    EXPECT_FALSE(build_time.empty());
    _prt("Code info: {}\n", _code_info());
    _prt("Build time: {}\n",_build_time());
}
