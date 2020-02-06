#include <iostream>
#include <mutex>

#include "dns_cache.hpp"

DNSCache::DNSCache(size_t max_size) : _max_size(max_size)
{
    if(_max_size > 0)
    {
        _dns_resolve.reserve(_max_size);
    }
    else
    {
        std::cerr << "Incorrect size for DNSCache = " << _max_size << "\n"
                  << "Used default = "<< CACHE_SIZE_DEF << "\n";
    }
}
//------------------------------------------------------------------------------

void DNSCache::update(const std::string& name, const std::string& ip)
{
    std::unique_lock lock(_mutex);
    auto dns_iter = _dns_resolve.find(name);
    // try update cache
    if(dns_iter != _dns_resolve.end())
    {
        // remove old data
        _dns.erase(dns_iter->second);
        // update dns info
        insert(name, ip);
    }
    else
    {
        if(_dns.size() == _max_size)
        {
            // erase old data
            _dns_resolve.erase(_dns.front().first);
            _dns.pop_front();
        }
        // update dns info
        insert(name, ip);
    }
}
//------------------------------------------------------------------------------

std::string DNSCache::resolve(const std::string& name) const
{
    std::shared_lock lock(_mutex);
    auto dns_iter = _dns_resolve.find(name);
    if( dns_iter != _dns_resolve.end() )
        return (dns_iter->second)->second;
    return "";
}
//------------------------------------------------------------------------------

size_t DNSCache::size() const
{
    std::shared_lock lock(_mutex);
    return _dns.size();
}

void DNSCache::reinit(size_t new_size)
{
    std::unique_lock lock(_mutex);
    _max_size = new_size;
    _dns.clear();
    _dns_resolve.clear();
    _dns_resolve.reserve(_max_size);
}
//------------------------------------------------------------------------------

void DNSCache::insert(const std::string& name, const std::string& ip)
{
    _dns.emplace_back(name, ip);
    _dns_resolve[name] = std::prev(_dns.end());
}
//------------------------------------------------------------------------------