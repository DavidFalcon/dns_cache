cmake_minimum_required(VERSION 3.2)
project(DNSCache)

add_subdirectory(cache)
set(SOURCE_FILES main.cpp)

add_executable(dns_cache ${SOURCE_FILES})
target_link_libraries(dns_cache cache_impl pthread)
