#include "../x.h"
#include <gtest/gtest.h>

using namespace x;

TEST(StructTest, BasicOperations) {
    Struct s;
    
    // Test set/get
    s.add("int_val", 42);
    s.add("str_val", _s("hello"));
    s.add("float_val", 3.14);
    s.add("bool_val", true);
    
    EXPECT_EQ(s.get<i32>("int_val"), 42);
    EXPECT_EQ(s.get<str>("str_val"), "hello");
    EXPECT_DOUBLE_EQ(s.get<f64>("float_val"), 3.14);
    EXPECT_TRUE(s.get<bool>("bool_val"));
    
    // Test has/remove
    EXPECT_TRUE(s.has("int_val"));
    s.remove("int_val");
    EXPECT_FALSE(s.has("int_val"));

    Struct s2 = s; // Test copy constructor
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s.size(), 3);
    EXPECT_TRUE(s2.has("str_val"));
    EXPECT_TRUE(s2.has("float_val"));

    Struct s3 = std::move(s2); // Test move constructor
    EXPECT_EQ(s3.size(), 3);
    EXPECT_FALSE(s2.has("str_val")); // s2 should be empty after move
    EXPECT_TRUE(s3.has("str_val"));
    EXPECT_TRUE(s3.has("float_val"));
    EXPECT_TRUE(s3.has("bool_val"));
    EXPECT_EQ(s3.size(), 3);
    EXPECT_TRUE(s2.empty());

    Struct s4 = {
        {"key1", 200},
        {"key2", _s("example")},
        {"key3", 1.414}
    }; // Test initializer list constructor
    EXPECT_EQ(s4.get<i32>("key1"), 200);
    EXPECT_EQ(s4.get<str>("key2"), "example");
    EXPECT_DOUBLE_EQ(s4.get<f64>("key3"), 1.414);
    EXPECT_EQ(s4.size(), 3);
}

TEST(StructTest, TypeConversion) {
    Struct s;
    s.add("int", 42);
    s.add("float", 3.14);
    s.add("str", _s("123"));
    
    // Test valid conversions
    EXPECT_EQ(s.get<i32>("int"), 42);
    EXPECT_EQ(s.get<f64>("float"), 3.14);
    EXPECT_EQ(s.get<str>("str"), "123");
    
    // Test invalid conversions
    EXPECT_THROW(s.get<bool>("str"), std::runtime_error);
    EXPECT_THROW(s.get<std::vector<int>>("int"), std::runtime_error);
}

TEST(StructTest, ThreadSafety) {
    Struct shared;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&shared, i]() {
            shared.add("thread_" + std::to_string(i), i);
            EXPECT_EQ(shared.get<i32>("thread_" + std::to_string(i)), i);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(shared.size(), 10);
}

TEST(StructTest, LargeData) {
    Struct s;
    std::vector<int> largeVec(10000);
    std::iota(largeVec.begin(), largeVec.end(), 0);
    
    s.add("large", largeVec);
    EXPECT_EQ(s.get<std::vector<int>>("large").size(), 10000);
    EXPECT_EQ(s.get<std::vector<int>>("large")[9999], 9999);
}
