#ifndef DNSCACHE_DNSCACHE_H
#define DNSCACHE_DNSCACHE_H

#include "CMap.h"

#include <string>
#include <shared_mutex>
#include <mutex>

using DomainName_t = std::string;
using DNSRecord_t = std::string;


class DNSCache {

private:
	explicit DNSCache(std::size_t capacity);

	CMap<DomainName_t, DNSRecord_t> cMap;

	mutable std::shared_mutex mutex;

public:
	DNSCache(DNSCache &&) = delete;

	DNSCache(const DNSCache &) = delete;

	DNSCache &operator=(const DNSCache &) = delete;

	DNSCache &operator=(DNSCache &&) = delete;

	static DNSCache &getInstance(std::size_t capacity = 0);

	/**
	 * @brief Adds or updates a DNS cache entry.
	 *
	 * @param name domain name.
	 * @param dnsRecord IP address in text format.
	 */
	void update(const DomainName_t &name, const DNSRecord_t &dnsRecord);

	/**
	 * @brief Returns IP address by domain name. If the entry is not found, returns an empty string.
	 *
	 * @param name domain name.
	 * @return IP address.
	 */
	std::string resolve(const DomainName_t &name) const;

	/**
	 * @brief Returns DNSCache capacity.
	 *
	 * @return DNSCache capacity.
	 * */
	std::size_t getCapacity() const noexcept;

	/**
	 * @brief Returns DNSCache size.
	 *
	 * @return DNSCache size.
	 * */
	std::size_t getSize() const noexcept;

	/**
	 * @brief Remove all records from DNSCache.
	 *
	 * */
	void clear();

	virtual ~DNSCache() = default;

};

#endif //DNSCACHE_DNSCACHE_H
