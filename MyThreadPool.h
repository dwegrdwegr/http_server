#include "MyThread.h"
#include "ThreadSafeQueue.h"
#include <vector>
#ifndef _MYTHREADPOOL_H
#define _MYTHREADPOOL_H

class MyThreadPool
{
        ThreadSafeQueue queue;
        std::vector<MyThread> threads;
        int count;
    public:
        friend class MyThread;
        MyThreadPool();
        ~MyThreadPool();
        void start_threads();
        template <class T>
        void add_work( T && work )
        {
            queue.push( std::forward<T>( work ) );
        }
};

#endif // _MYTHREADPOOL_H
