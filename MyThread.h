#include <pthread.h>
#ifndef _MYTHREAD_H
#define _MYTHREAD_H
class MyThreadPool;

class MyThread
{
    public:
        MyThread( MyThreadPool* arg );
        int join();
        static void* run( void* arg );
    private:
        pthread_t pthread;
        bool execute;
};
#endif
