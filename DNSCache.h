#ifndef DNSCACHE_DNSCACHE_H
#define DNSCACHE_DNSCACHE_H

#include <string>
#include "CMap.h"
#include "LockGuard/WLockGuard.h"
#include "LockGuard/RLockGuard.h"
#include <mutex>

using DomainName_t = std::string;
using DNSRecord_t = std::string;

static inline constexpr std::size_t DNSCacheCapacity = 4;

class DNSCache {

private:
    DNSCache() = default;
    CMap<DomainName_t, DNSRecord_t> cMap{DNSCacheCapacity};

    mutable pthread_rwlock_t rwlock;

    void releaseRWLock();

public:
    DNSCache(DNSCache&&) = delete;
    DNSCache(const DNSCache&) = delete;
    DNSCache& operator=(const DNSCache&) = delete;
    DNSCache& operator=(DNSCache&&) = delete;

    static DNSCache& getInstance();

    /**
     * Adds or updates a DNS cache entry.
     *
     * @param name domain name.
     * @param dnsRecord IP address in text format.
     */
    void update(const DomainName_t& name, const DNSRecord_t & dnsRecord);

    /**
     * Returns IP address by domain name. If the entry is not found, returns an empty string.
     *
     * @param name domain name.
     * @return IP address.
     */
    std::string resolve(const DomainName_t& name) const;

    /**
     * Returns DNSCache capacity.
     *
     * @return DNSCache capacity.
     * */
    std::size_t getCapacity() const;

    /**
     * Returns DNSCache size.
     *
     * @return DNSCache size.
     * */
    std::size_t getSize() const;

    /**
     * Remove all records from DNSCache.
     *
     * */
    void clear();

    virtual ~DNSCache();

};

#endif //DNSCACHE_DNSCACHE_H
