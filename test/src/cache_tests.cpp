#include "dns_cache.hpp"
#include "gtest/gtest.h"

class TestDNS : public ::testing::Test
{
protected:
    size_t size() const { return DNS().size(); };
    size_t max_size() const { return DNS().max_size(); };
    void reinit(size_t new_size = DNSCache::CACHE_SIZE_DEF) { DNS().reinit(new_size); };
};

TEST_F(TestDNS, singleton)
{
    DNSCache::init(10);
    ASSERT_EQ(max_size(), 10);

    DNSCache::init(100);
    ASSERT_EQ(max_size(), 10);

    auto& inst = DNS();
    reinit(20);
    ASSERT_EQ(&inst, &DNS());
}

TEST_F(TestDNS, DefaultSize)
{
    DNSCache::init(3);
    ASSERT_EQ(size(), 0);

    reinit(0);
    ASSERT_EQ(max_size(), DNSCache::CACHE_SIZE_DEF);
}

TEST_F(TestDNS, Size)
{
    reinit(2);
    ASSERT_EQ(size(), 0);

    DNS().update("a", "1");
    ASSERT_EQ(size(), 1);

    DNS().update("b", "2");
    ASSERT_EQ(size(), 2);

    DNS().update("c", "3");
    ASSERT_EQ(size(), 2);
}

TEST_F(TestDNS, Update)
{
    reinit(3);

    DNS().update("a", "1");
    DNS().update("b", "2");
    DNS().update("c", "3");
    DNS().update("d", "4");

    ASSERT_EQ(DNS().resolve("a"), "");
    ASSERT_EQ(DNS().resolve("b"), "2");
    ASSERT_EQ(DNS().resolve("f"), "");
    ASSERT_EQ(DNS().resolve("d"), "4");
}

TEST_F(TestDNS, UpResolve)
{
    reinit(3);

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

TEST_F(TestDNS, UpUpdate)
{
    reinit(3);

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