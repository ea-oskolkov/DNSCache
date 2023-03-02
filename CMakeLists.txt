cmake_minimum_required(VERSION 3.24)
project(DNSCache)

set(CMAKE_CXX_STANDARD 17)

add_executable(DNSCache
        main.cpp
        CMap.h
        DNSCache.cpp
        DNSCache.h
        LockGuard/RLockGuard.cpp
        LockGuard/RLockGuard.h
        LockGuard/WLockGuard.cpp
        LockGuard/WLockGuard.h
        Test/Test.cpp
        Test/Test.h)
