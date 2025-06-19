#include "x.hpp"
#include "gtest/gtest.h"

namespace {

TEST(SystemTest, ProcessIdValid) {
    auto pid = x::process_id();
    EXPECT_NE(pid, 0) << "Process ID should not be zero";
}

TEST(SystemTest, ProcessIdConsistent) {
    auto pid1 = x::process_id();
    auto pid2 = x::process_id();
    EXPECT_EQ(pid1, pid2) << "Process ID should be consistent between calls";
}

} // namespace
