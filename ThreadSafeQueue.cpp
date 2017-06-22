#include <exception>
#include "ThreadSafeQueue.h"
#include "Work.h"
#include "MyMutexGuard.h"

ThreadSafeQueue::ThreadSafeQueue( )
{
    pthread_cond_init( &condition_variable, NULL );
    pthread_mutex_init( &mutex, NULL );
}

ThreadSafeQueue::~ThreadSafeQueue( )
{
    pthread_mutex_destroy( &mutex );
    pthread_cond_destroy( &condition_variable );
}

Work ThreadSafeQueue::pop( )
{
    MyMutexGuard guard(mutex);
    while ( queue.empty() )
    {
        int result = pthread_cond_wait( &condition_variable, &mutex );
        if ( result != 0 )
        {
            throw std::bad_exception( );
        }
    };
    Work item = queue.front( );
    queue.pop( );
    return item;
}

void ThreadSafeQueue::push( const Work& item )
{
    MyMutexGuard guard(mutex);;
    queue.push( item );
    (void) pthread_cond_signal( &condition_variable );
}

void ThreadSafeQueue::push( Work && item )
{
    MyMutexGuard guard(mutex);
    queue.push( std::move( item ) );
    (void) pthread_cond_signal( &condition_variable );
}
