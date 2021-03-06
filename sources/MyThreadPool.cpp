#include "MyThreadPool.h"
#include <utility>
#include <unistd.h>

MyThreadPool::MyThreadPool( ThreadSafeQueue* q )
{
    count = 1; //sysconf( _SC_NPROCESSORS_ONLN );
    for ( int i = 0; i < count; i++ )
    {
        MyThread thread( q );
        threads.push_back( thread );
    }
}

void MyThreadPool::start_threads( )
{
    for ( auto& t : threads )
    {
        t.create( );
    }
}

MyThreadPool::~MyThreadPool( )
{
    for ( auto& t : threads )
    {
        t.cancel( );
    }
}
