#include <pthread.h>
#include "Work.h"
#ifndef _MYTHREAD_H
#define _MYTHREAD_H
class MyThreadPool;

class MyThread
{
    public:
        MyThread( MyThreadPool* parent );
        int join();
        int cancel();
        void create();
    private:
        void run();      
        static void* start( void* arg );
        pthread_t pthread;
        MyThreadPool* parent;
        bool execute;
};
#endif
