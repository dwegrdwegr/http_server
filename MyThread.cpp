#include "MyThread.h"
#include "MyThreadPool.h"
#include "HttpRequests.h"
#include "HttpResponse.h"
#include "HttpParser.h"
#include <exception>

MyThread::MyThread( MyThreadPool* parent )
{
    this->parent = parent;
}

int MyThread::join()
{
    return pthread_join( pthread, NULL );
}
int MyThread::cancel()
{
    return pthread_cancel( pthread );
}

void MyThread::run()
{
    HttpParser parser;
    while( true )
    {
        Work w = parent->queue.pop();
        std::unique_ptr<HttpRequest> req = parser.parse( w.http_string );
        HttpResponse response = req->process();
        w.socket_fd.send( response.to_string() );
    }
}

void MyThread::create()
{
    int result = pthread_create( &pthread, nullptr, MyThread::start, static_cast<void *>( this ) );
    if ( result != 0 )
    {
        throw std::bad_exception();
    }
}
void* MyThread::start( void* arg )
{
    MyThread* this_thread = static_cast<MyThread *>( arg );
    this_thread->run();
    //pthread_exit( nullptr );
}
