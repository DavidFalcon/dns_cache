#pragma once

#include <shared_mutex>
#include <unordered_map>

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

    struct Node
    {
        std::string ip;
        const std::string* prev = nullptr;
        const std::string* next = nullptr;

        Node(const std::string& ip_ = "") : ip(ip_) {}
    };
    using DNSInfo = std::unordered_map<std::string, Node>;

    void up(DNSInfo::iterator& it);
    void up_head(DNSInfo::iterator& it);
    void pop_back();
    void insert(const std::string& name, const std::string& ip);

private:
    size_t _max_size;
    const std::string* _head = nullptr;
    const std::string* _tail = nullptr;
    DNSInfo _dns;
    mutable std::shared_mutex _mutex;
};