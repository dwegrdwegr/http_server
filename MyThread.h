#include <pthread.h>
#include "ThreadSafeQueue.h"
#include "Work.h"
#ifndef _MYTHREAD_H
#define _MYTHREAD_H

class MyThread
{
public:
    MyThread( );
    int join( );
    int cancel( );
    void create( ThreadSafeQueue* q );
private:
    void run( );
    static void* start( void* arg );
    pthread_t pthread;
    ThreadSafeQueue* work_queue;
    bool execute;
};
#endif
