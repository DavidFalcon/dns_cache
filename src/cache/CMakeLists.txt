cmake_minimum_required(VERSION 3.2)
project(cache_impl C CXX)

set(SOURCE_FILES
    dns_cache.hpp
    dns_cache.cpp
)

add_library(cache_impl SHARED STATIC ${SOURCE_FILES})
