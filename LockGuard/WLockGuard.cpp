#include "WLockGuard.h"
#include <stdexcept>

WLockGuard::WLockGuard(pthread_rwlock_t *pthreadRwlock):pthreadRwlock(pthreadRwlock) {
    if (pthread_rwlock_wrlock(pthreadRwlock) !=0)
        throw std::runtime_error("pthread_rwlock_wrlock return error");
}

WLockGuard::~WLockGuard() {
    pthread_rwlock_unlock(pthreadRwlock);
}
