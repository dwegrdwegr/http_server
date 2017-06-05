#include <queue>
#include <pthread.h>
#include "Work.h"
#ifndef _THREADSAFEQUEUE_H
#define _THREADSAFEQUEUE_H

class ThreadSafeQueue
{
    public:
        Work pop();
        void push( Work item );
        void push( Work&& item );
    private:
        std::queue<Work> queue;
        pthread_cond_t condition_variable;
        pthread_mutex_t mutex;
};
#endif
