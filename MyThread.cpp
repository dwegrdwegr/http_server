#include "MyThread.h"
#include "MyThreadPool.h"
#include "HttpRequests.h"
#include "HttpParser.h"
#include <exception>

MyThread::MyThread( MyThreadPool* parent )
{
    this->parent = parent;
    int result = pthread_create( &pthread, nullptr, MyThread::start, static_cast<void *>( this ) );
    if ( result != 0 )
    {
        throw std::bad_exception();
    }
}

int MyThread::join()
{
    return pthread_join( pthread, NULL );
}

void MyThread::run()
{
    HttpParser parser;
    while( true )
    {
        Work w = parent->queue.pop();
        std::unique_ptr<HttpRequest> req = parser.parse( w.http_string );
        req->process( w.socket_fd );
    }
}
void* MyThread::start( void* arg )
{
    MyThread* this_thread = static_cast<MyThread *>( arg );
    this_thread->run();
    pthread_exit( nullptr );
}
