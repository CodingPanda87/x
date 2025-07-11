#include "../x.hpp"
#include <gtest/gtest.h>

using namespace x;

TEST(TimeTest, ConstructionFromComponents) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456);
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 8);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);
    EXPECT_EQ(t1.microseconds(), 123456);
}

TEST(TimeTest, ConstructionFromString) {
    Time t1("2024-06-19 08:08:08.123456");
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 8);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);
    EXPECT_EQ(t1.microseconds(), 123456);

    // Test with partial microseconds
    Time t2("2024-06-19 08:08:08.123");
    EXPECT_EQ(t2.microseconds(), 123000);
}

TEST(TimeTest, Now) {
    Time before = Time::now();
    sleep(10); // sleep 10ms
    Time after = Time::now();
    EXPECT_GT(after, before);
}

TEST(TimeTest, ToString) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456);
    EXPECT_EQ(t1.to_string(), "2024-06-19 08:08:08.123456");

    Time t2(2024, 6, 19, 8, 8, 8);
    EXPECT_EQ(t2.to_string(), "2024-06-19 08:08:08");
}

TEST(TimeTest, ComparisonOperators) {
    Time t1(2024, 6, 19, 8, 8, 8);
    Time t2(2024, 6, 19, 8, 8, 8);
    Time t3(2024, 6, 19, 8, 8, 9);

    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 != t2);
    EXPECT_TRUE(t1 < t3);
    EXPECT_TRUE(t1 <= t2);
    EXPECT_TRUE(t3 > t1);
    EXPECT_TRUE(t1 >= t2);
}

TEST(TimeTest, ArithmeticOperators) {
    Time t1(2024, 6, 19, 8, 8, 8);
    auto duration = std::chrono::microseconds(1000000); // 1 second
    
    Time t2 = t1 + duration;
    EXPECT_EQ(t2.second(), 9);
    
    Time t3 = t2 - duration;
    EXPECT_EQ(t3, t1);
    
    auto diff = t2 - t1;
    EXPECT_EQ(diff.count(), 1000000);
}

TEST(TimeTest, ComponentAccessors) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456);
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 8);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);
    EXPECT_EQ(t1.microseconds(), 123456);
}
