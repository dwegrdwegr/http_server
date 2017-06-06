#include "MyThread.h"
#include "MyThreadPool.h"
#include <exception>
MyThread::MyThread( MyThreadPool* arg )
{
    int result = pthread_create( &pthread, nullptr, MyThread::run, ( void * )arg );
    if ( result != 0 )
    {
        throw std::bad_exception();
    }
}

int MyThread::join()
{
    return pthread_join( pthread, NULL );
}

void* MyThread::run( void* arg )
{

}
