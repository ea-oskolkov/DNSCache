#ifndef DNSCACHE_RLOCKGUARD_H
#define DNSCACHE_RLOCKGUARD_H

#include <pthread.h>

/**
 * RAII wrapper over pthread_rwlock_rdlock/pthread_rwlock_unlock.
 *
 * */
class RLockGuard {

private:
    pthread_rwlock_t* pthreadRwlock;

public:
    explicit RLockGuard(pthread_rwlock_t* pthreadRwlock);

    virtual ~RLockGuard();
};


#endif //DNSCACHE_RLOCKGUARD_H
