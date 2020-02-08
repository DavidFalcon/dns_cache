cmake_minimum_required(VERSION 3.2)
project(cache_tests)

add_subdirectory(lib/googletest)

include_directories(${HEADERS_DIR})
include_directories(lib/googletest/googletest/include)

set(SOURCE_FILES main.cpp src/cache_tests.cpp src/perf_tests.cpp)

add_executable(cache_tests ${SOURCE_FILES})
target_link_libraries(cache_tests cache_impl gtest)

