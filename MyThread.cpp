#include "MyThread.h"
#include "MyThreadPool.h"
#include "HttpRequest.h"
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
        HttpRequest req = parser.parse( w.http_string );
        switch( req.method )
        {
            case method_type::GET:
                processGet( w );
                break;
            case method_type::POST:
                processPost( w );
                break;
        }
    }
}
void* MyThread::start( void* arg )
{
    MyThread* this_thread = static_cast<MyThread *>( arg );
    this_thread->run();
    pthread_exit( nullptr );
}
void MyThread::processGet( const Work& work )
{
    // generate html and send somehow
}
void MyThread::processPost( const Work& work )
{
    // generate html and send somehow
}
