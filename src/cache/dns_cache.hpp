#pragma once

#include <list>
#include <unordered_map>
#include <shared_mutex>

class DNSCache
{
public:

    explicit DNSCache(size_t max_size);

    void update(const std::string& name, const std::string& ip);

    std::string resolve(const std::string& name);

private:

    void insert(const std::string& name, const std::string& ip);

    using DNSInfo      = std::pair<std::string, std::string>;
    using CacheList    = std::list<DNSInfo>;
    using CacheResolve = std::unordered_map<std::string, CacheList::iterator>;

private:

    size_t _max_size;
    CacheList _dns;
    CacheResolve _dns_resolve;
    std::shared_mutex _mutex;
};