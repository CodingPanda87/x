#include "../x.hpp"
#include <gtest/gtest.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace x;

class FilesystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir = "test_fs_temp";
        test_file = x::join_path({test_dir, "test.txt"});
        x::create_dir(test_dir);
        x::write_file(test_file, "test content");
    }

    void TearDown() override {
        x::remove_dir(test_dir);
    }

    str test_dir;
    str test_file;
};

TEST_F(FilesystemTest, TestExists) {
    EXPECT_TRUE(x::exists(test_dir));
    EXPECT_TRUE(x::exists(test_file));
    EXPECT_FALSE(x::exists("nonexistent"));
}

TEST_F(FilesystemTest, TestIsFileAndDir) {
    EXPECT_TRUE(x::is_dir(test_dir));
    EXPECT_FALSE(x::is_file(test_dir));
    
    EXPECT_TRUE(x::is_file(test_file));
    EXPECT_FALSE(x::is_dir(test_file));
}

TEST_F(FilesystemTest, TestReadWriteFile) {
    str content = x::read_file(test_file);
    EXPECT_EQ(content, "test content");

    x::write_file(test_file, "new content");
    EXPECT_EQ(x::read_file(test_file), "new content");
}

TEST_F(FilesystemTest, TestPathOperations) {
    _vec<str> parts = {"a", "b", "c.txt"};
    str joined = x::join_path(parts);
#ifdef _WIN32
    EXPECT_EQ(joined, "a\\b\\c.txt");
#else
    EXPECT_EQ(joined, "a/b/c.txt");
#endif

    _vec<str> split = x::split_path(joined);
    EXPECT_EQ(split.size(), 3);
    EXPECT_EQ(split[0], "a");
    EXPECT_EQ(split[1], "b");
    EXPECT_EQ(split[2], "c.txt");

    str norm = x::norm_path("a/./b/../c.txt");
#ifdef _WIN32
    EXPECT_EQ(norm, "a\\c.txt");
#else
    EXPECT_EQ(norm, "a/c.txt");
#endif
}

TEST_F(FilesystemTest, TestFileSize) {
    EXPECT_GT(x::file_size(test_file), 0);
}

TEST_F(FilesystemTest, TestListDir) {
    _vec<str> entries = x::list_dir(test_dir);
    EXPECT_FALSE(entries.empty());
    EXPECT_TRUE(x::contain(entries[0], "test.txt"));
}

TEST_F(FilesystemTest, TestRemoveOperations) {
    str temp_file = x::join_path({test_dir, "temp.txt"});
    x::write_file(temp_file, "temp");
    EXPECT_TRUE(x::exists(temp_file));
    
    x::remove_file(temp_file);
    EXPECT_FALSE(x::exists(temp_file));

    str temp_dir = x::join_path({test_dir, "subdir"});
    x::create_dir(temp_dir);
    EXPECT_TRUE(x::exists(temp_dir));
    
    x::remove_dir(temp_dir);
    EXPECT_FALSE(x::exists(temp_dir));
}

TEST_F(FilesystemTest, TestFilename) {
    EXPECT_EQ(x::file_name("path/to/file.txt"), "file.txt");
    EXPECT_EQ(x::file_name("file.txt"), "file.txt");
    EXPECT_EQ(x::file_name(""), "");
    EXPECT_EQ(x::file_name("path/with.no.extension"), "with.no.extension");
    EXPECT_EQ(x::file_name("path/with.dot."), "with.dot.");
    EXPECT_EQ(x::file_name(test_file), "test.txt");
    EXPECT_EQ(x::file_name("path/to/file.ext"), "file.ext");
    EXPECT_EQ(x::file_name("path/"), "");
    EXPECT_EQ(x::file_name("/"), "");
#ifdef _WIN32
    EXPECT_EQ(x::file_name("\\"), "");
#else
    EXPECT_EQ(x::file_name("\\"), "\\");
#endif
    EXPECT_EQ(x::file_name("a/b/c.tar.gz"), "c.tar.gz");
    
    // Test Unicode filenames
    EXPECT_EQ(x::file_name("path/测试文件.txt"), "测试文件.txt");
    EXPECT_EQ(x::file_name("path/こんにちは世界.txt"), "こんにちは世界.txt");
}

TEST_F(FilesystemTest, TestPathOperationsExtended) {
#ifdef _WIN32
    // Test more complex path joining
    EXPECT_EQ(x::join_path({"a", "b", "..", "c"}), "a\\b\\..\\c");
    EXPECT_EQ(x::join_path({".", "a", "b"}), ".\\a\\b");
    EXPECT_EQ(x::join_path({"a", "", "b"}), "a\\b");  // Empty path component
    
    // Test path normalization edge cases
    EXPECT_EQ(x::norm_path("a/./b/.././c"), "a\\c");
#else
    // Test more complex path joining
    EXPECT_EQ(x::join_path({"a", "b", "..", "c"}), "a/b/../c");
    EXPECT_EQ(x::join_path({".", "a", "b"}), "./a/b");
    EXPECT_EQ(x::join_path({"a", "", "b"}), "a/b");  // Empty path component
    
    // Test path normalization edge cases
    EXPECT_EQ(x::norm_path("a/./b/.././c"), "a/c");
#endif
    EXPECT_EQ(x::norm_path("./a/../"), ".");
    EXPECT_EQ(x::norm_path(""), "");
}

TEST_F(FilesystemTest, TestErrorCases) {
    // Test operations on non-existent paths
    EXPECT_FALSE(x::is_file("nonexistent_file"));
    EXPECT_FALSE(x::is_dir("nonexistent_dir"));
    EXPECT_EQ(x::file_size("nonexistent"), 0);
    EXPECT_TRUE(x::read_file("nonexistent").empty());
    
    // Test invalid operations
    x::remove_file("nonexistent");
    x::remove_dir("nonexistent");
    EXPECT_TRUE(x::list_dir("nonexistent").empty());
}

TEST_F(FilesystemTest, TestFileTime) {
    // Test file_time function with existing file
    str time_str = x::file_time(test_file);
    EXPECT_FALSE(time_str.empty());
    
    // Verify the format matches YYYY-MM-DD HH:MM:SS
    EXPECT_EQ(time_str.length(), 19); // "YYYY-MM-DD HH:MM:SS" is 19 characters
    
    // Check that it has the correct format pattern
    EXPECT_EQ(time_str[4], '-');
    EXPECT_EQ(time_str[7], '-');
    EXPECT_EQ(time_str[10], ' ');
    EXPECT_EQ(time_str[13], ':');
    EXPECT_EQ(time_str[16], ':');
    
    // Test file_time function with nonexistent file
    str nonexistent_time = x::file_time("nonexistent_file");
    EXPECT_TRUE(nonexistent_time.empty());
}
