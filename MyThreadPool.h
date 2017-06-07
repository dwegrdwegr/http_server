#include "MyThread.h"
#include "ThreadSafeQueue.h"
#include <vector>
#ifndef _MYTHREADPOOL_H
#define _MYTHREADPOOL_H

class MyThreadPool
{
        std::vector<MyThread> threads;
        ThreadSafeQueue queue;
        uint8_t count;
    public:
        friend class MyThread;
        MyThreadPool();
        ~MyThreadPool();
        template <typename T>
        void add_work( T&& work );
};

#endif // _MYTHREADPOOL_H
