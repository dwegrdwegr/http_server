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
        void run();
        void processGet( const Work& work );
        void processPost( const Work& work );
        static void* start( void* arg );
    private:
        pthread_t pthread;
        MyThreadPool* parent;
        bool execute;
};
#endif
