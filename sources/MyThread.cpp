#include "MyThread.h"
#include "HttpRequests.h"
#include "HttpResponse.h"
#include "HttpParser.h"
#include "jsmn.h"
#include <exception>
#include "MyServerSocket.h"
#include <sys/select.h>
#include <vector>

MyBasicThread::MyBasicThread( )
{

}

int MyBasicThread::join( ) noexcept
{
    return pthread_join( pthread, NULL );
}

int MyBasicThread::cancel( ) noexcept
{
    return pthread_cancel( pthread );
}

MyRegistrationThread::MyRegistrationThread( MyServerSocket* s )
{
    socket = s;
}

void MyRegistrationThread::register_users( const std::string& json )
{
    jsmn_parser parser;
    jsmn_init( &parser );
    int a = jsmn_parse( &parser, json.c_str(), json.length( ), NULL, 1000 );
    
}

void MyRegistrationThread::run( )
{
    std::vector<MyServerSocket> connected;
    int nfds = socket->get_socket( );
    fd_set fds;
    FD_ZERO( &fds );
    FD_SET( socket->get_socket( ), &fds );
    while ( true )
    {
        for ( auto it = connected.begin( ); it != connected.end( ); ++it )
        {
            FD_SET( it->get_socket( ), &fds );
            if ( it->get_socket( ) >= nfds )
            {
                nfds = it->get_socket( ) + 1;
            }
        }

        if ( select( nfds + 1, &fds, NULL, NULL, NULL ) > 0 )
        {
            if ( FD_ISSET( socket->get_socket( ), &fds ) )
            {
                int fd = socket->accept( );
                MyServerSocket sck( fd );
                connected.push_back( sck );
            }
            else
            {
                auto it = connected.begin( );
                auto end = connected.end( );
                while ( it != end )
                {
                    if ( FD_ISSET( it->get_socket( ), &fds ) )
                    {
                        FD_CLR( it->get_socket( ), &fds );
                        int amount = it->recv( );
                        if ( amount > 0 )
                        {
                            std::string received( it->get_buffer( amount ) );
                            it->clear_buffer( );
                            register_users( received );
                            ++it;
                        }
                        else if ( amount == 0 ) // connection is closed
                        {
                            connected.erase( it++ );
                        }
                    }
                }
            }
        }
    }
}

MyThread::MyThread( ThreadSafeQueue* q )
{
    work_queue = q;
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
            response = generate_bad_request( );
        }
        w.socket_fd.send( response.to_string( ) );
    }
}

void MyBasicThread::create( )
{
    int result = pthread_create( &pthread, nullptr, MyBasicThread::start, static_cast < void * > ( this ) );
    if ( result != 0 )
    {
        throw std::bad_exception( );
    }
}

void* MyBasicThread::start( void* arg )
{
    MyBasicThread* this_thread = static_cast < MyBasicThread * > ( arg );
    this_thread->run( );
    return NULL;
}
