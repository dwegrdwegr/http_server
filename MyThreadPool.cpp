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
}

template <typename T>
void MyThreadPool::add_work( T&& work )
{
    queue.push( std::forward<T>( work ) );
}
