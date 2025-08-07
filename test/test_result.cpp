#include "../x.hpp"
#include <gtest/gtest.h>

using namespace x;

TEST(ResultTest, DefaultConstructor) {
    Result r;
    EXPECT_FALSE(r.ok());
    EXPECT_EQ(r.code(), 0xFFFFFFFF);
    EXPECT_TRUE(r.message().empty());
    EXPECT_FALSE(static_cast<bool>(r));
}

TEST(ResultTest, ParameterizedConstructor) {
    Result r1(0, "success");
    EXPECT_TRUE(r1.ok());
    EXPECT_EQ(r1.code(), 0);
    EXPECT_EQ(r1.message(), "success");
    EXPECT_TRUE(static_cast<bool>(r1));

    Result r2(404, "not found");
    EXPECT_FALSE(r2.ok());
    EXPECT_EQ(r2.code(), 404);
    EXPECT_EQ(r2.message(), "not found");
    EXPECT_FALSE(static_cast<bool>(r2));
}

TEST(ResultTest, SetResultBasic) {
    Result r;
    r.setResult(42);
    EXPECT_TRUE(r.ok());
    EXPECT_EQ(r.code(), 0);
    EXPECT_EQ(r.dat1<int>(), 42);
}

TEST(ResultTest, SetResultMultiple) {
    Result r;
    r.setResult(42, _s("hello"), 3.14);
    EXPECT_TRUE(r.ok());
    EXPECT_EQ(r.dat1<int>(), 42);
    EXPECT_EQ(r.dat2<std::string>(), "hello");
    EXPECT_DOUBLE_EQ(r.dat3<double>(), 3.14);
}

TEST(ResultTest, DataAccess) {
    Result r;
    r.setResult(std::vector{1, 2, 3}, _s("test"), false);
    
    auto vec = r.dat1<std::vector<int>>();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    
    EXPECT_EQ(r.dat2<std::string>(), "test");
    EXPECT_FALSE(r.dat3<bool>());
}

TEST(ResultTest, EdgeCases) {
    Result r1(0, "");
    EXPECT_TRUE(r1.ok());
    EXPECT_TRUE(r1.message().empty());

    Result r2;
    r2.setResult(nullptr);
    EXPECT_EQ(r2.dat1<std::nullptr_t>(), nullptr);
}

TEST(ResultTest, InvalidCast) {
    Result r;
    r.setResult(42);
    
    EXPECT_THROW(r.dat1<std::string>(), std::bad_any_cast);
    EXPECT_THROW(r.dat2<int>(), std::bad_any_cast);
    EXPECT_THROW(r.dat3<double>(), std::bad_any_cast);
}

TEST(ResultTest, ComplexDataTypes) {
    Result r;
    std::vector<std::map<std::string, int>> complexData = {
        {{"a", 1}, {"b", 2}},
        {{"c", 3}, {"d", 4}}
    };
    r.setResult(complexData);
    
    auto retrieved = r.dat1<decltype(complexData)>();
    EXPECT_EQ(retrieved.size(), 2);
    EXPECT_EQ(retrieved[0]["a"], 1);
    EXPECT_EQ(retrieved[1]["d"], 4);
}

TEST(ResultTest, MoveSemantics) {
    Result r1;
    r1.setResult(std::vector<int>{1, 2, 3});
    
    Result r2(std::move(r1));
    EXPECT_EQ(r2.dat1<std::vector<int>>().size(), 3);
    EXPECT_FALSE(r1.has_data1());
    
    Result r3;
    r3 = std::move(r2);
    EXPECT_EQ(r3.dat1<std::vector<int>>().size(), 3);
    EXPECT_FALSE(r2.has_data2());
}

TEST(ResultTest, CopyBehavior) {
    Result r1;
    r1.setResult(_s("original"));
    
    Result r2(r1);
    EXPECT_EQ(r2.dat1<std::string>(), _s("original"));
    
    Result r3;
    r3 = r1;
    EXPECT_EQ(r3.dat1<std::string>(), _s("original"));
    
    // Modify original shouldn't affect copies
    r1.setResult("modified");
    EXPECT_EQ(r2.dat1<std::string>(), _s("original"));
    EXPECT_EQ(r3.dat1<std::string>(), _s("original"));
}

TEST(ResultTest, NestedResults) {
    Result inner;
    inner.setResult(42);
    
    Result outer;
    outer.setResult(inner);
    
    auto retrieved = outer.dat1<Result>();
    EXPECT_EQ(retrieved.dat1<int>(), 42);
}

TEST(ResultTest, ErrorPropagation) {
    Result r1(404, "Not found");
    Result r2;
    r2.setResult(r1);
    
    auto retrieved = r2.dat1<Result>();
    EXPECT_FALSE(retrieved.ok());
    EXPECT_EQ(retrieved.code(), 404);
    EXPECT_EQ(retrieved.message(), "Not found");
}
