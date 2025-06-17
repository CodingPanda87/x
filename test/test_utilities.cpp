#include "../x.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <algorithm>

using namespace x;

class UtilitiesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UtilitiesTest, TestTimeFunctions) {
    str time_str = x::current_time();
    _prtln("{}",time_str);
    EXPECT_FALSE(time_str.empty());
    
    u64 ts1 = x::timestamp();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    u64 ts2 = x::timestamp();
    EXPECT_GT(ts2, ts1);
}

TEST_F(UtilitiesTest, TestRandomNumbers) {
    // Test rand_int
    i32 num = x::rand_int(1, 100);
    EXPECT_GE(num, 1);
    EXPECT_LE(num, 100);
    
    // Test multiple calls
    _vec<i32> nums;
    for (int i = 0; i < 100; ++i) {
        nums.push_back(x::rand_int(1, 100));
    }
    auto [min, max] = std::minmax_element(nums.begin(), nums.end());
    EXPECT_GE(*min, 1);
    EXPECT_LE(*max, 100);
    
    // Test rand_float
    f64 fnum = x::rand_float(0.5, 1.5);
    EXPECT_GE(fnum, 0.5);
    EXPECT_LE(fnum, 1.5);
}

TEST_F(UtilitiesTest, TestClamp) {
    EXPECT_EQ(x::clamp(5, 1, 10), 5);
    EXPECT_EQ(x::clamp(0, 1, 10), 1);
    EXPECT_EQ(x::clamp(11, 1, 10), 10);
    
    // Test with floats
    EXPECT_DOUBLE_EQ(x::clamp(3.14, 0.0, 5.0), 3.14);
    EXPECT_DOUBLE_EQ(x::clamp(-1.0, 0.0, 5.0), 0.0);
    EXPECT_DOUBLE_EQ(x::clamp(6.0, 0.0, 5.0), 5.0);
}

TEST_F(UtilitiesTest, TestThreadId) {
    u64 id1 = x::id_thread();
    u64 id2 = x::id_thread();
    EXPECT_EQ(id1, id2); // Same thread should have same ID
    
    std::thread t([&]() {
        u64 id3 = x::id_thread();
        EXPECT_NE(id1, id3); // Different thread should have different ID
        EXPECT_NE(id3, 0);   // ID should not be zero
    });
    t.join();
}

TEST_F(UtilitiesTest, TestSleep) {
    auto start = std::chrono::steady_clock::now();
    x::sleep(50); // Sleep for 50ms
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_GE(duration.count(), 50);
    
    // Test very short sleep
    start = std::chrono::steady_clock::now();
    x::sleep(1); // Sleep for 1ms
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_GE(duration.count(), 1);
}

TEST_F(UtilitiesTest, TestRandomDistribution) {
    const int samples = 10000;
    std::vector<int> counts(10, 0);
    
    for (int i = 0; i < samples; ++i) {
        int num = x::rand_int(0, 9);
        EXPECT_GE(num, 0);
        EXPECT_LE(num, 9);
        counts[num]++;
    }
    
    // Check distribution is roughly uniform
    for (int count : counts) {
        EXPECT_GT(count, samples/20); // Each bucket should have at least 5% of samples
        EXPECT_LT(count, samples/5);  // And no more than 20%
    }
}

TEST_F(UtilitiesTest, TestClampEdgeCases) {
    // Test equal min/max
    EXPECT_EQ(x::clamp(5, 5, 5), 5);
    EXPECT_EQ(x::clamp(4, 5, 5), 5);
    EXPECT_EQ(x::clamp(6, 5, 5), 5);
    
    EXPECT_EQ(x::clamp(-0.000001, 0.0, 1.0), 0.0);
}

TEST_F(UtilitiesTest, TestThreadIdUniqueness) {
    std::mutex mtx;
    std::set<u64> ids;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back([&]() {
            u64 id = x::id_thread();
            std::lock_guard<std::mutex> lock(mtx);
            EXPECT_EQ(ids.count(id), 0); // Each ID should be unique
            ids.insert(id);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
