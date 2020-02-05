cmake_minimum_required(VERSION 3.3)

project("DNSCache")

set(CMAKE_CONFIGURATION_TYPES Debug Release)

if(NOT BUILD_TYPE)
    message(STATUS "Setting default build type 'Devel'")
    set(BUILD_TYPE "Debug")
elseif(NOT (${BULD_TYPE} IN_LIST CMAKE_CONFIGURATION_TYPES))
    message(FATAL_ERROR "Unknown build type (${BUILD_TYPE})")
endif()

set(CMAKE_BUILD_TYPE "${BUILD_TYPE}")
message(STATUS "Build type '${BUILD_TYPE}'")

set(CMAKE_C_FLAGS_DEVEL "-O2")
set(CMAKE_CXX_FLAGS_DEVEL "-O2")

set(CMAKE_C_FLAGS_RELEASE "-O2 -D_RELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -D_RELEASE")

add_compile_options(-std=gnu++17 -Wall)

set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE  SOURCE_FILES
    "${SOURCE_PATH}/*.h"
    "${SOURCE_PATH}/*.cc"
    "${SOURCE_PATH}/*.cpp"
    "${SOURCE_PATH}/*.hpp"
)

add_executable(dns_cache ${SOURCE_FILES})

target_link_libraries(dns_cache pthread)
target_include_directories(dns_cache PUBLIC ${SOURCE_PATH})