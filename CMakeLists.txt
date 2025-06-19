cmake_minimum_required(VERSION 3.10)
project(x-lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Enable testing
enable_testing()

# Download and include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://gitee.com/mirrors/googletest.git #https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # 使用动态运行时库
FetchContent_MakeAvailable(googletest)

# Add your executable and test files
add_executable(${PROJECT_NAME}_test
          test/test_basic.cpp
          test/test_string_ops.cpp
          test/test_filesystem.cpp
          test/test_utilities.cpp
          test/test_result.cpp
          test/test_timer.cpp
          test/test_struct.cpp
          test/test_time.cpp
          )

add_executable(${PROJECT_NAME}_demo
            demo.cpp
          )

# Link with GoogleTest
target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest_main)

# Add test
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})
