#include <exception>
#include "ThreadSafeQueue.h"
#include "Work.h"

Work ThreadSafeQueue::pop()
{
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
    return item;
}

void ThreadSafeQueue::push( const Work& item )
{
    pthread_mutex_lock( &mutex );
    queue.push( item );
    pthread_mutex_unlock( &mutex );
    pthread_cond_signal( &condition_variable );
}

void ThreadSafeQueue::push( Work&& item )
{
    pthread_mutex_lock( &mutex );
    queue.push( std::move( item ) );
    pthread_mutex_unlock( &mutex );
    pthread_cond_signal( &condition_variable );
}
