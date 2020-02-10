#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>

#include "dns_cache.hpp"
#include "gtest/gtest.h"

namespace Performance
{

using Dns = std::pair<std::string, std::string>;

constexpr size_t max_size = 100000;
constexpr size_t thread_count = 10;

std::vector<Dns> dns;
std::vector<std::thread> pool;

std::string random_string( size_t length )
{
    auto randchar = []()
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void fill_v()
{
    dns.reserve(max_size);
    for(size_t i = 0; i < max_size; ++i)
        dns.push_back({random_string(10), random_string(10)});
}

void fill_dns()
{
    for(const auto& [name, ip] : dns)
    {
        DNSCache::inst().update(name,ip);
    }
}

void test_cache()
{
    for(size_t val = max_size -1; val >= max_size/2; --val)
    {
        auto dns_value = dns.at(rand() % (max_size - val) + val);
        DNSCache::inst().update(dns_value.first, dns_value.second);

        dns_value = dns.at(rand() % (max_size - val) + val);
        DNSCache::inst().resolve(dns_value.first);
    }
}

TEST(DNS, Performance)
{
    srand (time(NULL));
    DNSCache::inst().reinit(max_size/2);

    fill_v();
    fill_dns();

    auto start = std::chrono::high_resolution_clock::now();

    for(size_t i = 0; i < thread_count; ++i)
        pool.push_back(std::thread(test_cache));

    for(size_t i = 0; i < thread_count; ++i)
        pool[i].join();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;

    ASSERT_GT(5000, elapsed.count());
}

}