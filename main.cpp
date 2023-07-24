#include "./Test/Test.h"
#include "DNSCache.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

	try {
		std::cout << "Testing DNS cache with " << Test::capacityDNSCache << " items" << std::endl;

		auto &dnsCache = DNSCache::getInstance(Test::capacityDNSCache);

		Test::oneThreadTest(&dnsCache);

		dnsCache.clear();

		Test::multiThreadTest(&dnsCache);
	}
	catch (const std::exception &ex) {
		std::cerr << "Test failed" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
