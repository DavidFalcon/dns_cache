#include "dns_cache.hpp"
#include "gtest/gtest.h"

TEST(DNS, singleton)
{
    DNSCache::init(10);
    ASSERT_EQ(DNS().max_size(), 10);

    DNSCache::init(100);
    ASSERT_EQ(DNSCache::inst().max_size(), 10);

    auto& inst = DNS();
    DNS().reinit(20);
    ASSERT_EQ(&inst, &DNS());
}

TEST(DNS, DefaultSize)
{
    DNSCache::init(3);
    ASSERT_EQ(DNS().size(), 0);

    DNS().reinit(0);
    ASSERT_EQ(DNS().max_size(), DNSCache::CACHE_SIZE_DEF);
}

TEST(DNS, Size)
{
    DNS().reinit(2);
    ASSERT_EQ(DNS().size(), 0);

    DNS().update("a", "1");
    ASSERT_EQ(DNS().size(), 1);

    DNS().update("b", "2");
    ASSERT_EQ(DNS().size(), 2);

    DNS().update("c", "3");
    ASSERT_EQ(DNS().size(), 2);
}

TEST(DNS, Update)
{
    DNS().reinit(3);

    DNS().update("a", "1");
    DNS().update("b", "2");
    DNS().update("c", "3");
    DNS().update("d", "4");

    ASSERT_EQ(DNS().resolve("a"), "");
    ASSERT_EQ(DNS().resolve("b"), "2");
    ASSERT_EQ(DNS().resolve("f"), "");
    ASSERT_EQ(DNS().resolve("d"), "4");
}

TEST(DNS, UpResolve)
{
    DNS().reinit(3);

    DNS().update("a", "1");
    DNS().update("b", "2");
    DNS().update("c", "3");
    DNS().resolve("a");
    DNS().update("d", "4");

    ASSERT_EQ(DNS().resolve("a"), "1");
    ASSERT_EQ(DNS().resolve("c"), "3");
    ASSERT_EQ(DNS().resolve("d"), "4");
    ASSERT_EQ(DNS().resolve("b"), "");
    ASSERT_EQ(DNS().resolve("f"), "");
}

TEST(DNS, UpUpdate)
{
    DNS().reinit(3);

    DNS().update("a", "1");
    DNS().update("b", "2");
    DNS().update("c", "3");
    DNS().update("a", "2");
    DNS().update("d", "4");

    ASSERT_EQ(DNS().resolve("a"), "2");
    ASSERT_EQ(DNS().resolve("c"), "3");
    ASSERT_EQ(DNS().resolve("d"), "4");
    ASSERT_EQ(DNS().resolve("b"), "");
    ASSERT_EQ(DNS().resolve("f"), "");
}