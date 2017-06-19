#include "MyThread.h"
#include "ThreadSafeQueue.h"
#include <vector>
#ifndef _MYTHREADPOOL_H
#define _MYTHREADPOOL_H

class MyThreadPool
{
        std::vector<MyThread> threads;
        int count;
    public:
        friend class MyThread;
        MyThreadPool();
        ~MyThreadPool();
        void start_threads(ThreadSafeQueue* q);
};

#endif // _MYTHREADPOOL_H
