#include "Test.h"

#include <thread>
#include <iostream>
#include <limits>

namespace Test {
    struct TestCaseItem {
        DomainName_t name;
        DNSRecord_t record;
    };

    static const TestCaseItem testCaseArr[] = {
            {"zzz.com", "192.168.0.1"},
            {"www.com", "192.168.0.2"},
            {"qqq.com", "192.168.0.3"},
            {"eee.com", "192.168.0.4"},

            {"qqq.com", "192.168.0.5"},

            {"111.com", "192.168.0.6"},
            {"222.com", "192.168.0.7"},
            {"333.com", "192.168.0.8"}
    };

    static void checkEqualDNSRecord(const DNSRecord_t &expected, const DNSRecord_t &actual) {
        if (expected != actual) {
            std::cerr << "expected: " << actual << ", actual: " << expected << std::endl;
            throw std::runtime_error("test failure");
        }
    }

    static void checkEqualUint(const uint64_t expected, const uint64_t actual) {
        if (expected != actual) {
            std::cerr << "expected: " << actual << ", actual: " << expected << std::endl;
            throw std::runtime_error("test failure");
        }
    }

    void multiThreadTest(DNSCache &dnsCache) {
        std::cout << "Start multithreading test (~" << timeSecMultithreading << " sec)" << std::endl;
        std::cout << "Data consistency check during concurrent read/write" << std::endl;
        std::vector<std::thread> thVector;
        bool run = true;

        // Write threads
        for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            thVector.emplace_back([&] {
                try {
                    while (run) {
                        for (std::size_t i = 0; i < capacityDNSCache; ++i) {
                            const auto &item = testCaseArr[i];
                            dnsCache.update(item.name, item.record);
                        }

                        // Sleep 1ms
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                } catch (const std::exception &ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
            });
        }

        // Waiting for filling
        while (dnsCache.getSize() != capacityDNSCache);

        // Read threads
        for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            thVector.emplace_back([&] {
                try {
                    while (run) {
                        for (std::size_t i = 0; i < capacityDNSCache; ++i) {
                            const auto &item = testCaseArr[i];
                            const auto &actual = dnsCache.resolve(item.name);
                            const auto &expected = item.record;
                            checkEqualDNSRecord(expected, actual);
                            // Sleep 1ms
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        }
                    }
                } catch (const std::exception &ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
            });
        }

        // Block current thread on N second
        std::this_thread::sleep_for(std::chrono::seconds(timeSecMultithreading));

        // Stop all threads
        run = false;

        // Join all threads
        std::for_each(thVector.begin(), thVector.end(), [](std::thread &t) { t.join(); });

        std::cout << "Success" << std::endl;
    }

    void oneThreadTest(DNSCache &dnsCache) {

        try {
            /// Simply add and resolve (0-3)
            {
                std::cout << "Simply add and resolve" << std::endl;

                // add
                for (unsigned i = 0; i < 4; ++i)
                    dnsCache.update(testCaseArr[i].name, testCaseArr[i].record);

                // Resolve
                for (unsigned i = 0; i < 4; ++i) {
                    const auto &expected = testCaseArr[i].record;
                    const auto &actual = dnsCache.resolve(testCaseArr[i].name);
                    checkEqualDNSRecord(expected, actual);
                }
                checkEqualUint(dnsCache.getSize(), capacityDNSCache);
                std::cout << "Success" << std::endl << std::endl;
            }

            /// Update (4)
            {
                std::cout << "Update" << std::endl;
                dnsCache.update(testCaseArr[4].name, testCaseArr[4].record);

                const auto &expected = testCaseArr[4].record;
                const auto &actual = dnsCache.resolve(testCaseArr[4].name);
                checkEqualDNSRecord(expected, actual);
                checkEqualUint(dnsCache.getSize(), capacityDNSCache);

                std::cout << "Success" << std::endl << std::endl;
            }

            /// Overflow with removal of the oldest (5-7)
            {
                std::cout << "Overflow with removal of the oldest" << std::endl;

                for (unsigned i = 5; i < 8; ++i)
                    dnsCache.update(testCaseArr[i].name, testCaseArr[i].record);

                for (unsigned i = 4; i < 8; ++i) {
                    const auto &actual = dnsCache.resolve(testCaseArr[i].name);
                    const auto &expected = testCaseArr[i].record;
                    checkEqualDNSRecord(expected, actual);
                }
                checkEqualUint(dnsCache.getSize(), capacityDNSCache);
                std::cout << "Success" << std::endl << std::endl;
            }

            /// Resolve non-existent domain
            {
                const auto &actual = dnsCache.resolve("qwertyuiop");
                DNSRecord_t expected{};
                checkEqualDNSRecord(expected, actual);
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}