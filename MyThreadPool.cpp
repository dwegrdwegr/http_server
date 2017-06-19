#include "MyThreadPool.h"
#include <utility>
#include <unistd.h>

MyThreadPool::MyThreadPool( )
{
    count = 1; //sysconf( _SC_NPROCESSORS_ONLN );
    for ( int i = 0; i < count; i++ )
    {
        MyThread thread;
        threads.push_back( thread );
    }
}

void MyThreadPool::start_threads( ThreadSafeQueue* q)
{
    for ( auto& t : threads )
    {
        t.create(q);
    }
}

MyThreadPool::~MyThreadPool( )
{
    for ( auto& t : threads )
    {
        t.cancel( );
    }
}
