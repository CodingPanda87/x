#include "../x.h"
#include <gtest/gtest.h>
#include <atomic>

TEST(TimerTest, OneTimeTimer) {
    x::Timer timer;
    std::atomic<int> counter{0};
    
    timer.start(100, [&counter]() {
        counter++;
    });
    
    EXPECT_TRUE(timer.is_running());
    x::sleep(150);
    EXPECT_EQ(counter, 1);
    EXPECT_FALSE(timer.is_running());
}

TEST(TimerTest, RepeatingTimer) {
    x::Timer timer;
    std::atomic<int> counter{0};
    
    timer.start(50, [&counter]() {
        counter++;
    }, true);
    
    EXPECT_TRUE(timer.is_running());
    x::sleep(220);
    timer.stop();
    EXPECT_GE(counter, 4); // Should fire ~4 times in 220ms
    EXPECT_FALSE(timer.is_running());
}

TEST(TimerTest, ManualStop) {
    x::Timer timer;
    std::atomic<int> counter{0};
    
    timer.start(50, [&counter]() {
        counter++;
    }, true);
    
    EXPECT_TRUE(timer.is_running());
    x::sleep(100);
    timer.stop();
    int final_count = counter;
    x::sleep(100);
    EXPECT_EQ(counter, final_count); // Shouldn't increment after stop
    EXPECT_FALSE(timer.is_running());
}

TEST(TimerTest, DestructorStopsTimer) {
    std::atomic<int> counter{0};
    {
        x::Timer timer;
        timer.start(50, [&counter]() {
            counter++;
        }, true);
        EXPECT_TRUE(timer.is_running());
        x::sleep(75);
    } // Timer destroyed here
    int final_count = counter;
    x::sleep(100);
    EXPECT_EQ(counter, final_count); // Shouldn't increment after destruction
}

TEST(TimerTest, PrecisionTiming) {
    x::Timer timer;
    std::atomic<int> counter{0};
    auto start = std::chrono::steady_clock::now();
    
    timer.start(100, [&counter, start]() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        EXPECT_NEAR((double)elapsed, 100, 15); // Allow 10ms tolerance
        counter++;
    });
    
    x::sleep(150);
    EXPECT_EQ(counter, 1);
}

TEST(TimerTest, MultipleTimers) {
    x::Timer timer1, timer2;
    std::atomic<int> counter1{0}, counter2{0};
    
    timer1.start(50, [&counter1]() { counter1++; }, true);
    timer2.start(75, [&counter2]() { counter2++; }, true);
    
    x::sleep(300);
    timer1.stop();
    timer2.stop();
    
    EXPECT_GE(counter1, 4); // ~50ms interval
    EXPECT_GE(counter2, 3); // ~75ms interval
    EXPECT_LT(counter1, 8); // Shouldn't run too many times
    EXPECT_LT(counter2, 6); // Shouldn't run too many times
}

TEST(TimerTest, ImmediateCancellation) {
    x::Timer timer;
    std::atomic<int> counter{0};
    
    timer.start(100, [&counter]() { counter++; });
    timer.stop();
    x::sleep(150);
    
    EXPECT_EQ(counter, 0);
}

TEST(TimerTest, ThreadSafety) {
    x::Timer timer;
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    
    // Start timer from one thread
    threads.emplace_back([&]() {
        timer.start(50, [&counter]() { counter++; }, true);
    });
    
    // Stop timer from another thread after delay
    threads.emplace_back([&]() {
        x::sleep(200);
        timer.stop();
    });
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_GT(counter, 0);
    EXPECT_LT(counter, 5); // Shouldn't run too many times
}

TEST(TimerTest, ResourceCleanup) {
    std::weak_ptr<void> weak_resource;
    {
        x::Timer timer;
        auto resource = std::make_shared<int>(42);
        weak_resource = resource;
        
        timer.start(50, [resource]() {
            // Hold resource in callback
            EXPECT_EQ(*resource, 42);
        }, true);
        
        x::sleep(75);
        timer.stop();
    }
    
    // Resource should be cleaned up after timer stops
    EXPECT_TRUE(weak_resource.expired());
}
