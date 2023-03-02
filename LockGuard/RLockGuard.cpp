#include "RLockGuard.h"
#include <stdexcept>

RLockGuard::RLockGuard(pthread_rwlock_t *pthreadRwlock):pthreadRwlock(pthreadRwlock) {
    if (pthread_rwlock_rdlock(pthreadRwlock) != 0)
        throw std::runtime_error("pthread_rwlock_rdlock return error");
}

RLockGuard::~RLockGuard() {
    pthread_rwlock_unlock(pthreadRwlock);
}