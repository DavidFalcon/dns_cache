#include "dns_cache.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

TEST(DNS, DefaultSize)
{
    DNSCache::init(3);
    ASSERT_EQ(DNSCache::inst().size(), 0);
}

TEST(DNS, Size)
{
    DNSCache::inst().reinit(2);
    ASSERT_EQ(DNSCache::inst().size(), 0);

    DNSCache::inst().update("a", "1");
    ASSERT_EQ(DNSCache::inst().size(), 1);

    DNSCache::inst().update("b", "2");
    ASSERT_EQ(DNSCache::inst().size(), 2);

    DNSCache::inst().update("c", "3");
    ASSERT_EQ(DNSCache::inst().size(), 2);
}

TEST(DNS, Update)
{
    DNSCache::inst().reinit(3);

    DNSCache::inst().update("a", "1");
    DNSCache::inst().update("b", "2");
    DNSCache::inst().update("c", "3");
    DNSCache::inst().update("d", "4");

    ASSERT_EQ(DNSCache::inst().resolve("a"), "");
    ASSERT_EQ(DNSCache::inst().resolve("b"), "2");
    ASSERT_EQ(DNSCache::inst().resolve("f"), "");
    ASSERT_EQ(DNSCache::inst().resolve("d"), "4");
}