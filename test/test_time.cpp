#include "../x.hpp"
#include <gtest/gtest.h>

using namespace std::literals;
using namespace x;

TEST(TimeTest, ConstructionFromComponents) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456);
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 0);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);

    EXPECT_EQ(t1.microseconds(), 123456);

    Time t2 = t1;
    EXPECT_EQ(t2, t1);
    EXPECT_EQ(t1.to_string(), t2.to_string());
}

TEST(TimeTest, ConstructionFromString) {
    Time t1("2024-06-19 08:08:08.123456");
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 0);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);
    EXPECT_EQ(t1.microseconds(), 123456);

    // Test with partial microseconds
    Time t2("2024-06-19 08:08:08.123");
    EXPECT_EQ(t2.microseconds(), 123);

    Time t3(2024, 6, 19, 8, 8, 8,123);
    EXPECT_TRUE(t2 == t3);
}

TEST(TimeTest, Now) {
    Time before = Time::now();
    x::sleep(10); // sleep 10ms
    Time after = Time::now();
    EXPECT_GT(after, before);
}

TEST(TimeTest, ToString) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456,false); // time zone
    EXPECT_EQ(t1.to_string(true), "2024-06-19 16:08:08.123456");
    Time t2(2024, 6, 19, 8, 8, 8,0,false); //time zone
    EXPECT_EQ(t2.to_string(true), "2024-06-19 16:08:08.000000");
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

    Time t4(2024, 6, 19, 8, 8, 8);
    auto t5 = t4 + 1000us;
    EXPECT_TRUE(t5 > t4);
}

TEST(TimeTest, ComponentAccessors) {
    Time t1(2024, 6, 19, 8, 8, 8, 123456);
    EXPECT_EQ(t1.year(), 2024);
    EXPECT_EQ(t1.month(), 6);
    EXPECT_EQ(t1.day(), 19);
    EXPECT_EQ(t1.hour(), 0);
    EXPECT_EQ(t1.minute(), 8);
    EXPECT_EQ(t1.second(), 8);
    EXPECT_EQ(t1.microseconds(), 123456);
}

TEST(TimeTest, TimeZoneConversion) {
    // Test local time to UTC conversion in constructor
    Time localTime(2024, 6, 19, 8, 0, 0); // Local time (isTimeZone=true)
    Time utcTime(2024, 6, 19, 0, 0, 0, 0, false); // UTC time
    
    // The difference should be equal to timezone offset
    auto offset = localTime - utcTime;
    EXPECT_TRUE(offset.count() == 0);
    
    // Test string constructor with timezone
    Time localFromStr("2024-06-19 08:00:00");
    auto s1 = localFromStr.to_string(false);
    auto s2 = utcTime.to_string(false);
    EXPECT_EQ(s1, s2);
}

TEST(TimeTest, UTCStringRepresentation) {
    Time utcTime(2024, 6, 19, 8, 0, 0, 0, false);
    std::string utcStr = utcTime.to_string(false);
    
    // UTC string should match input time exactly
    EXPECT_EQ(utcStr, "2024-06-19 08:00:00.000000");
    
    // Local string should be offset by timezone
    std::string localStr = utcTime.to_string(true);
    EXPECT_NE(localStr, utcStr);
}