#include <pthread.h>
#ifndef _MYMUTEXGUARD_H
#define _MYMUTEXGUARD_H

class MyMutexGuard
{
    pthread_mutex_t& mutex;
public:
    MyMutexGuard( pthread_mutex_t& m );
    MyMutexGuard( const MyMutexGuard& orig ) = delete;
    ~MyMutexGuard( );
};

#endif /* MYMUTEXGUARD_H */

