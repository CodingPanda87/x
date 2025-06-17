#include "x.hpp"
#include "gtest/gtest.h"

class FormatTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout/cerr for testing print functions
        cout_buf = std::cout.rdbuf();
        cerr_buf = std::cerr.rdbuf();
        std::cout.rdbuf(cout_ss.rdbuf());
        std::cerr.rdbuf(cerr_ss.rdbuf());
    }

    void TearDown() override {
        // Restore cout/cerr
        std::cout.rdbuf(cout_buf);
        std::cerr.rdbuf(cerr_buf);
    }

    std::streambuf* cout_buf;
    std::streambuf* cerr_buf;
    std::stringstream cout_ss;
    std::stringstream cerr_ss;
};

TEST_F(FormatTest, TestVfmt) {
    std::string result = x::vfmt("Hello {} {}", "world", 42);
    EXPECT_EQ(result, "Hello world 42");
}

TEST_F(FormatTest, TestVprt) {
    x::vprt("Test {} {}", 1, 2);
    EXPECT_EQ(cout_ss.str(), "Test 1 2");
    cout_ss.str("");
}

TEST_F(FormatTest, TestVprtln) {
    x::vprtln("Line {}", 1);
    EXPECT_EQ(cout_ss.str(), "Line 1\n");
    cout_ss.str("");
}

TEST_F(FormatTest, TestVeprt) {
    x::veprt("Error {} {}", "code", 500);
    EXPECT_EQ(cerr_ss.str(), "Error code 500");
    cerr_ss.str("");
}

TEST_F(FormatTest, TestVeprtln) {
    x::veprtln("Error {}", "occurred");
    EXPECT_EQ(cerr_ss.str(), "Error occurred\n");
    cerr_ss.str("");
}

TEST_F(FormatTest, TestNumericFormatting) {
    // Test integer formatting
    EXPECT_EQ(x::vfmt("{:d}", 42), "42");
    EXPECT_EQ(x::vfmt("{:x}", 255), "ff");
    EXPECT_EQ(x::vfmt("{:o}", 8), "10");
    EXPECT_EQ(x::vfmt("{:b}", 5), "101");
    
    // Test float formatting
    EXPECT_EQ(x::vfmt("{:.2f}", 3.14159), "3.14");
    EXPECT_EQ(x::vfmt("{:e}", 1000.0), "1.000000e+03");
    EXPECT_EQ(x::vfmt("{:g}", 1000000.0), "1e+06");
    
    // Test padding/alignment
    EXPECT_EQ(x::vfmt("{:5}", 42), "   42");
    EXPECT_EQ(x::vfmt("{:<5}", 42), "42   ");
    EXPECT_EQ(x::vfmt("{:^5}", 42), " 42  ");
    EXPECT_EQ(x::vfmt("{:*>5}", 42), "***42");
}

TEST_F(FormatTest, TestComplexFormatting) {
    // Test mixed types
    EXPECT_EQ(x::vfmt("{} {} {:.2f}", "Value:", 42, 3.14159), "Value: 42 3.14");
    
    // Test positional arguments
    EXPECT_EQ(x::vfmt("{1} {0}", "world", "Hello"), "Hello world");
    
    // Test nested formatting
    EXPECT_EQ(x::vfmt("{:{}}", "test", 6), "  test");
    
    // Test format specifiers
    EXPECT_EQ(x::vfmt("{:+}", 42), "+42");
    EXPECT_EQ(x::vfmt("{:04}", 42), "0042");
}

TEST_F(FormatTest, TestFormatErrors) {
    // Test invalid format strings
    EXPECT_THROW(x::vfmt("{", 42), std::runtime_error);
    EXPECT_THROW(x::vfmt("{} {}", 42), std::runtime_error);
    EXPECT_THROW(x::vfmt("{:invalid}", 42), std::runtime_error);
    
    // Test type mismatches
    EXPECT_THROW(x::vfmt("{:d}", "not_a_number"), std::runtime_error);
    EXPECT_THROW(x::vfmt("{:.2f}", "not_a_float"), std::runtime_error);
}

TEST_F(FormatTest, TestPerformanceSensitiveCases) {
    // Test large strings
    std::string large(1000, 'x');
    EXPECT_EQ(x::vfmt("{}", large), large);
    
    // Test many arguments
    EXPECT_EQ(x::vfmt("{} {} {} {} {}", 1, 2, 3, 4, 5), "1 2 3 4 5");
    
    // Test repeated formatting
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(x::vfmt("Iteration {}", i), "Iteration " + std::to_string(i));
    }
}
