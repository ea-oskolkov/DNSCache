#include "DNSCache.h"
#include "LockGuard/WLockGuard.h"
#include "LockGuard/RLockGuard.h"

DNSCache &DNSCache::getInstance() {
    // Meyers singleton
    // Thread-safe (with C++ or above)
    static DNSCache dnsCache;
    return dnsCache;
}

DNSCache::~DNSCache() {
    releaseRWLock();
}

void DNSCache::update(const DomainName_t& name, const DNSRecord_t & dnsRecord) {
    if (name.empty() || dnsRecord.empty())
        return;

    WLockGuard lock(&rwlock);
    cMap.add(name, dnsRecord); // Skip returns value
}

std::string DNSCache::resolve(const DomainName_t& name) const {
    if (name.empty())
        return {};

    RLockGuard lock(&rwlock);
    return cMap.get(name);
}

std::size_t DNSCache::getCapacity() const {
    return cMap.getCapacity();
}

std::size_t DNSCache::getSize() const {
    return cMap.getSize();
}

void DNSCache::clear() {
    WLockGuard lock(&rwlock);
    cMap.clear();
}

void DNSCache::releaseRWLock() {
    // Waiting for lock release
    while (pthread_rwlock_destroy(&rwlock) == EBUSY);
}
