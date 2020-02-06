cmake_minimum_required(VERSION 3.2)

project(cmake-project-cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")


set(HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/cache)

include_directories(${HEADERS_DIR})

add_subdirectory(src)
add_subdirectory(test)


