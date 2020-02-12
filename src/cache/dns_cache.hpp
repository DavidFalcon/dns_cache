#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "shared_m.hpp"

class DNSCache
{
public:

    /// public methods
    void update(const std::string& name, const std::string& ip);

    std::string resolve(const std::string& name);

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

    /// delete default constructors object
    DNSCache(DNSCache&&) = delete;
    DNSCache(const DNSCache&) = delete;
    DNSCache& operator=(DNSCache&&) = delete;
    DNSCache& operator=(const DNSCache&) = delete;

    static constexpr size_t CACHE_SIZE_DEF = 1000;

private:
    friend class TestDNS;
    friend class Performance;

    /// methods used only for gtests
    size_t size() const;
    size_t max_size() const;
    void reinit(size_t new_size = CACHE_SIZE_DEF);

private:

    explicit DNSCache(size_t max_size);

    struct Node
    {
        std::string ip;
        const std::string* prev = nullptr;
        const std::string* next = nullptr;

        Node(const std::string& ip_ = "") : ip(ip_) {}
    };
    using DNSInfo = std::unordered_map<std::string, Node>;

    void resize(size_t max_size);
    void up(DNSInfo::iterator& it);
    void up_head(DNSInfo::iterator& it);
    void pop_back();
    void insert(const std::string& name, const std::string& ip);

private:
    size_t _max_size;
    const std::string* _head = nullptr;
    const std::string* _tail = nullptr;
    DNSInfo _dns;
    mutable sf::contention_free_shared_mutex<> _mutex;
    //mutable std::shared_mutex _mutex;
};

constexpr auto DNS = DNSCache::inst;