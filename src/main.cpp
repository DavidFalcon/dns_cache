#include <iostream>

#include "dns_cache.hpp"

int main(int argc, char ** argv)
{
    DNSCache cache(3);

    cache.update("a", "1");
    cache.update("b", "2");
    cache.update("c", "3");
    cache.update("d", "4");

    std::cout << cache.resolve("a") << "\n" << 
                 cache.resolve("b") << "\n" << 
                 cache.resolve("f") << "\n" <<
                 cache.resolve("d") << "\n";

    return 0;
}