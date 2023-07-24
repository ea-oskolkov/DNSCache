#ifndef DNSCACHE_TEST_H
#define DNSCACHE_TEST_H

#include "../DNSCache.h"

namespace Test {
	static inline constexpr std::size_t capacityDNSCache = 4;
	static inline constexpr std::size_t timeSecMultithreading = 15;

	void multiThreadTest(DNSCache *dnsCache);

	void oneThreadTest(DNSCache *dnsCache);
}

#endif //DNSCACHE_TEST_H
