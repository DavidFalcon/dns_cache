#include <iostream>

#include "dns_cache.hpp"

int main(int argc, char ** argv)
{
    DNSCache::init(3);

    DNSCache::inst().update("a", "1");
    DNSCache::inst().update("b", "2");
    DNSCache::inst().update("c", "3");
    DNSCache::inst().update("d", "4");

    std::cout << DNSCache::inst().resolve("a") << " \n" <<
                 DNSCache::inst().resolve("b") << " \n" <<
                 DNSCache::inst().resolve("f") << " \n" <<
                 DNSCache::inst().resolve("d") << " \n";

    return 0;
}