#include "MyThreadPool.h"
#include <utility>
#include <unistd.h>

MyThreadPool::MyThreadPool()
{
    count = sysconf( _SC_NPROCESSORS_ONLN );
    for( int i = 0; i < count; i ++ )
    {
        MyThread thread( this );
        threads.push_back( thread );
    }
}

MyThreadPool::~MyThreadPool()
{
    for( auto t : threads)
    {
        t.cancel();
    }
}
