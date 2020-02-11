#include <iostream>

#include "dns_cache.hpp"

int main(int argc, char ** argv)
{
    DNSCache::init(3);

    DNS().update("a", "1");
    DNS().update("b", "2");
    DNS().update("c", "3");
    DNS().update("d", "4");

    std::cout << DNS().resolve("a") << " \n" <<
                 DNS().resolve("b") << " \n" <<
                 DNS().resolve("f") << " \n" <<
                 DNS().resolve("d") << " \n";

    return 0;
}