#include "MyThread.h"
#include "HttpRequests.h"
#include "HttpResponse.h"
#include "HttpParser.h"
#include <exception>

MyThread::MyThread( )
{
}

int MyThread::join( ) noexcept
{
    return pthread_join( pthread, NULL );
}

int MyThread::cancel( ) noexcept
{
    return pthread_cancel( pthread );
}

void MyThread::run( )
{
    HttpParser parser;
    while ( true )
    {
        Work w = work_queue->pop( );
        HttpResponse response;
        try
        {
            std::unique_ptr<HttpRequest> req = parser.parse( w.http_string );
            response = req->process( );
        }
        catch ( std::exception& e )
        {
            response = generate_bad_request();
        }
        w.socket_fd.send( response.to_string( ) );
    }
}

void MyThread::create( ThreadSafeQueue* q )
{
    work_queue = q;
    int result = pthread_create( &pthread, nullptr, MyThread::start, static_cast < void * > ( this ) );
    if ( result != 0 )
    {
        throw std::bad_exception( );
    }
}

void* MyThread::start( void* arg )
{
    MyThread* this_thread = static_cast < MyThread * > ( arg );
    this_thread->run( );
    return NULL;
}
