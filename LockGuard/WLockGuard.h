#ifndef DNSCACHE_WLOCKGUARD_H
#define DNSCACHE_WLOCKGUARD_H

#include <pthread.h>

/**
 * RAII wrapper over pthread_rwlock_wrlock/pthread_rwlock_unlock.
 *
 * */
class WLockGuard {

private:
    pthread_rwlock_t* pthreadRwlock;

public:
    explicit WLockGuard(pthread_rwlock_t* pthreadRwlock);

    virtual ~WLockGuard();
};


#endif //DNSCACHE_WLOCKGUARD_H
