#include <exception>
#include "ThreadSafeQueue.h"
#include "Work.h"
#include <errno.h>
ThreadSafeQueue::ThreadSafeQueue()
{
    pthread_cond_init( &condition_variable, NULL );
    pthread_mutex_init( &mutex, NULL );
}
ThreadSafeQueue::~ThreadSafeQueue()
{
    pthread_mutex_destroy( &mutex );
    pthread_cond_destroy( &condition_variable );
}
Work ThreadSafeQueue::pop()
{ 
    int a = queue.size();
    printf("Dupa");
    pthread_mutex_lock( &mutex );
    while( queue.empty() )
    {
        int result = pthread_cond_wait( &condition_variable, &mutex );
        if( result != 0 )
        {
            throw std::bad_exception();
        }
    };
    Work item = queue.front();
    //queue.pop_front();
    queue.pop();
    if( pthread_mutex_unlock( &mutex ) != 0 )
    {
        perror( "Mutex unlock pop" );
    }
    return item;
}

void ThreadSafeQueue::push( const Work& item )
{
    pthread_mutex_lock( &mutex );
    //queue.push_front(item);
    queue.push( item );
    pthread_mutex_unlock( &mutex );
    pthread_cond_signal( &condition_variable );
}

void ThreadSafeQueue::push( Work && item )
{
    pthread_mutex_lock( &mutex );
    //queue.push_front(item);
    queue.push( std::move( item ) );
    pthread_cond_signal( &condition_variable );
    pthread_mutex_unlock( &mutex );
    //int a = 
}
