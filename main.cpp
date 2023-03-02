#include <iostream>
#include "DNSCache.h"
#include "./Test/Test.h"

int main() {

    std::cout << "Testing DNS cache with " << DNSCacheCapacity << " items" << std::endl;

    auto& dnsCache= DNSCache::getInstance();

    Test::oneThreadTest(dnsCache);

    dnsCache.clear();

    Test::multiThreadTest(dnsCache);

    return 0;
}
