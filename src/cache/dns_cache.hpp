#pragma once

#include <list>
#include <unordered_map>
#include <shared_mutex>

class DNSCache
{
public:

    static constexpr size_t CACHE_SIZE_DEF = 1000;

    /// singleton access
    static DNSCache& init(size_t max_size)
    {
        static DNSCache inst(max_size);
        return inst;
    }

    static DNSCache& inst()
    {
        return init(CACHE_SIZE_DEF);
    }

    /// public methods
    void update(const std::string& name, const std::string& ip);

    std::string resolve(const std::string& name) const;

    size_t size() const;

    /// reinit used only for gtests
    void reinit(size_t new_size = CACHE_SIZE_DEF);

    /// delete default constructors object
    DNSCache(DNSCache&&) = delete;
    DNSCache(const DNSCache&) = delete;
    DNSCache& operator=(DNSCache&&) = delete;
    DNSCache& operator=(const DNSCache&) = delete;

private:

    explicit DNSCache(size_t max_size);

    void insert(const std::string& name, const std::string& ip);

private:
    using DNSInfo      = std::pair<std::string, std::string>;
    using CacheList    = std::list<DNSInfo>;
    using CacheResolve = std::unordered_map<std::string, CacheList::iterator>;

    size_t _max_size;
    CacheList _dns;
    CacheResolve _dns_resolve;
    mutable std::shared_mutex _mutex;
};