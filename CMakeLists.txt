#https://cmake.org/cmake/help/latest/command/cmake_policy.html#command:cmake_policy
# "...The <min> version must be at least 2.4 and at most the running version of CMake."
cmake_minimum_required(VERSION 2.4...2.8)
project(DNSCache)

set(CMAKE_CXX_STANDARD 17)

add_executable(DNSCache
        main.cpp
        CMap.h
        DNSCache.cpp
        DNSCache.h
        Test/Test.cpp
        Test/Test.h
)
