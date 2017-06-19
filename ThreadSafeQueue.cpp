#include <exception>
#include "ThreadSafeQueue.h"
#include "Work.h"

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
    (void) pthread_mutex_lock( &mutex );
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
    (void) pthread_mutex_unlock( &mutex );  
    return item;
}

void ThreadSafeQueue::push( const Work& item )
{
    (void) pthread_mutex_lock( &mutex );
    queue.push( item );
    (void) pthread_mutex_unlock( &mutex );
    (void) pthread_cond_signal( &condition_variable );
}

void ThreadSafeQueue::push( Work && item )
{
    (void) pthread_mutex_lock( &mutex );
    queue.push( std::move( item ) );
    (void) pthread_cond_signal( &condition_variable );
    (void) pthread_mutex_unlock( &mutex );
}
