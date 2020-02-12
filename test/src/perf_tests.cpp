#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>

#include "dns_cache.hpp"
#include "gtest/gtest.h"

class Performance : public ::testing::Test
{
protected:

    Performance()
    {
        srand (time(NULL));
        reinit(max_size/2);

        fill_v();
        fill_dns();
    }

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
            DNS().update(name,ip);
        }
    }

    static void test_cache()
    {
        for(size_t val = max_size -1; val >= max_size/2; --val)
        {
            auto dns_value = dns.at(rand() % (max_size - val) + val);
            DNS().update(dns_value.first, dns_value.second);

            dns_value = dns.at(rand() % (max_size - val) + val);
            DNS().resolve(dns_value.first);
        }
    }

    static void run()
    {
        for(size_t i = 0; i < thread_count; ++i)
            pool.push_back(std::thread(test_cache));

        for(size_t i = 0; i < thread_count; ++i)
            pool[i].join();
    }

    void reinit(size_t new_size = DNSCache::CACHE_SIZE_DEF) { DNS().reinit(new_size); };

protected:
    using Dns = std::pair<std::string, std::string>;

    static constexpr size_t max_size = 100000;
    static constexpr size_t thread_count = 10;

    static std::vector<Dns> dns;
    static std::vector<std::thread> pool;
};

/// Initialize static member of class Performance
std::vector<Performance::Dns> Performance::dns;
std::vector<std::thread>      Performance::pool;

TEST_F (Performance, DNSPerf)
{
    auto start = std::chrono::high_resolution_clock::now();

    run();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;

    ASSERT_GT(5000, elapsed.count());
}