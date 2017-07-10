#include "MyServer.h"
#include <sys/select.h>
#include <vector>

MyServer::MyServer( ) :
    pool( &work_queue ),
    socket( static_cast < unsigned short > ( 8080 ) ),
    registration_socket( static_cast < unsigned short > ( 1337 ) ),
    registration_thread( &registration_socket )
{

}

MyServer::~MyServer( )
{
}

void MyServer::run( )
{
    try
    {
        pool.start_threads( );
    }
    catch ( std::exception& e )
    {
        printf( "Could not start threads" );
        return;
    }
    std::vector<MyServerSocket> connected;
    fd_set fds;
    int nfds;
    try
    {
        registration_socket.bind( );
        registration_socket.listen( 128 );
        socket.bind( );
        socket.listen( 128 );
    }
    catch ( std::exception& e )
    {
        printf( "Could not start the server" );
        return;
    }
    registration_thread.create( );
    nfds = socket.get_socket( );
    FD_ZERO( &fds );
    FD_SET( socket.get_socket( ), &fds );
    while ( true )
    {
        if ( !connected.empty( ) )
        {
            for ( auto it = connected.begin( ); it != connected.end( ); ++it )
            {
                FD_SET( it->get_socket( ), &fds );
                if ( it->get_socket( ) >= nfds )
                {
                    nfds = it->get_socket( );
                }
            }
        }
        else
        {
            nfds = socket.get_socket( );
        }

        if ( select( nfds + 1, &fds, NULL, NULL, NULL ) > 0 )
        {
            if ( FD_ISSET( socket.get_socket( ), &fds ) )
            {
                int fd = socket.accept( );
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
                            Work w{ *it, received };
                            work_queue.push( std::move( w ) );
                            ++it;
                        }
                        else if ( amount == 0 ) // connection is closed
                        {
                            FD_CLR( it->get_socket( ), &fds );
                            it->close( );
                            connected.erase( it++ );

                        }
                    }
                }
            }
        }
    }
}
