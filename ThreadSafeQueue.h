#include <deque>
#include <pthread.h>
#include "Work.h"
#ifndef _THREADSAFEQUEUE_H
#define _THREADSAFEQUEUE_H

class ThreadSafeQueue
{
    public:
        ThreadSafeQueue();
        ~ThreadSafeQueue();
        ThreadSafeQueue( const ThreadSafeQueue& ) = delete;
        ThreadSafeQueue& operator=( const ThreadSafeQueue& ) = delete;
        Work pop();
        void push( const Work& item );
        void push( Work && item );
    private:
        std::deque<Work> queue;
        pthread_cond_t condition_variable;
        pthread_mutex_t mutex;
};
#endif
