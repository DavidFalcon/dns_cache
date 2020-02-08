#include <iostream>
#include <mutex>
#include <cassert>

#include "dns_cache.hpp"

DNSCache::DNSCache(size_t max_size) : _max_size(max_size)
{
    if(_max_size > 0)
    {
        _dns.reserve(_max_size);
    }
    else
    {
        std::cerr << "Incorrect size for DNSCache = " << _max_size << "\n"
                  << "Used default = "<< CACHE_SIZE_DEF << "\n";
        _dns.reserve(CACHE_SIZE_DEF);
    }
}
//------------------------------------------------------------------------------

void DNSCache::update(const std::string& name, const std::string& ip)
{
    std::unique_lock lock(_mutex);

    auto dns_iter = _dns.find(name);
    /// try update cache
    if(dns_iter != _dns.end())
    {
        (dns_iter->second).ip = ip;
        up(dns_iter);
    }
    else
    {
        if(_dns.size() == _max_size)
            pop_back();

        insert(name, ip);
    }
}
//------------------------------------------------------------------------------

std::string DNSCache::resolve(const std::string& name) const
{
    std::shared_lock lock(_mutex);

    auto dns_iter = _dns.find(name);
    if( dns_iter != _dns.end() )
        return (dns_iter->second).ip;
    return "";
}
//------------------------------------------------------------------------------

size_t DNSCache::size() const
{
    std::shared_lock lock(_mutex);
    return _dns.size();
}
//------------------------------------------------------------------------------

void DNSCache::reinit(size_t new_size)
{
    std::unique_lock lock(_mutex);
    _max_size = new_size;
    _dns.clear();
    _dns.reserve(_max_size);
}
//------------------------------------------------------------------------------

void DNSCache::up(DNSInfo::iterator& it)
{
    if(it->second.prev)
    {
        auto node_prev = _dns[*it->second.prev];
        node_prev.next = it->second.next;

        if(it->second.next)
        {
            auto node_next = _dns[*it->second.next];
            node_next.prev = it->second.prev;
        }
        else
        {
            _tail = it->second.prev;
        }

        up_head(it);
    }
}
//------------------------------------------------------------------------------

void DNSCache::up_head(DNSInfo::iterator& it)
{
    assert(_head);

    auto node_head = _dns[*_head];
    node_head.prev = &(it->first);
    it->second.next = _head;
    _head = &(it->first);
}
//------------------------------------------------------------------------------

void DNSCache::pop_back()
{
    assert(_tail);

    auto node_tail = _dns[*_tail];
    if(node_tail.prev) // [[likely]] in c++20
    {
        auto node_prev = _dns.find(*node_tail.prev);
        node_prev->second.next = nullptr;
        _tail = &(node_prev->first);
    }
    _dns.erase(*_tail);
    _tail = nullptr;
}
//------------------------------------------------------------------------------

void DNSCache::insert(const std::string& name, const std::string& ip)
{
    auto it = _dns.emplace(name, ip);

    if(_dns.size() != 1) // [[likely]] in c++20
    {
        up_head(it.first);
    }
    else
    {
        _head = _tail = &(it.first->first);
    }
}
//------------------------------------------------------------------------------