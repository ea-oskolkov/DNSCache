#include "DNSCache.h"

DNSCache &DNSCache::getInstance(std::size_t capacity) {
    // Meyers singleton
    // Thread-safe (with C++ or above)
    static DNSCache dnsCache(capacity);
    return dnsCache;
}

void DNSCache::update(const DomainName_t& name, const DNSRecord_t & dnsRecord) {
    if (name.empty() || dnsRecord.empty())
        return;

    std::lock_guard<std::shared_mutex> lock(mutex);
    cMap.add(name, dnsRecord); // Skip returns value
}

std::string DNSCache::resolve(const DomainName_t& name) const {
    if (name.empty())
        return {};

    std::shared_lock<std::shared_mutex> lock(mutex);
    return cMap.get(name);
}

std::size_t DNSCache::getCapacity() const {
    return cMap.getCapacity();
}

std::size_t DNSCache::getSize() const {
    return cMap.getSize();
}

void DNSCache::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex);
    cMap.clear();
}

DNSCache::DNSCache(std::size_t capacity): cMap(capacity) {}
