#include <exception>
#include "ThreadSafeQueue.h"
#include "Work.h"
ThreadSafeQueue::ThreadSafeQueue()
{
    condition_variable = PTHREAD_COND_INITIALIZER;
    mutex = PTHREAD_MUTEX_INITIALIZER;
}
ThreadSafeQueue::~ThreadSafeQueue()
{
}
Work ThreadSafeQueue::pop()
{
    ( void )pthread_mutex_lock( &mutex );
    while( queue.empty() )
    {
        int result = pthread_cond_wait( &condition_variable, &mutex );
        if( result != 0 )
        {
            throw std::bad_exception();
        }
    };
    Work item = queue.front();
    queue.pop();
    ( void )pthread_mutex_unlock( &mutex );
    return item;
}

void ThreadSafeQueue::push( const Work& item )
{
    pthread_mutex_lock( &mutex );
    queue.push( item );
    pthread_cond_signal( &condition_variable );
    pthread_mutex_unlock( &mutex );
}

void ThreadSafeQueue::push( Work && item )
{
    pthread_mutex_lock( &mutex );
    queue.push( std::move( item ) );
    pthread_cond_signal( &condition_variable );
    pthread_mutex_unlock( &mutex );
}
