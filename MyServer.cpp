#include "MyServer.h"
#include <sys/select.h>
#include <vector>
MyServer::MyServer(): socket( static_cast<unsigned short>( 8080 ) )
{

}

MyServer::~MyServer()
{
}
void MyServer::run()
{
    std::vector<MyServerSocket> connected;
    fd_set fds;
    int nfds;
    socket.bind();
    socket.listen( 10 );
    nfds = socket.get_socket();
    FD_ZERO( &fds );
    FD_SET( socket.get_socket(), &fds );
    while( true )
    {
        for ( auto it = connected.begin(); it != connected.end(); ++it )
        {
            FD_SET( it->get_socket(), &fds );
            if ( it->get_socket() >= nfds )
            {
                nfds = it->get_socket() + 1;
            }
        }

        if( select( nfds + 1, &fds, NULL, NULL, NULL ) > 0 )
        {
            if( FD_ISSET( socket.get_socket(), &fds ) )
            {
                int fd = socket.accept();
                MyServerSocket sck( fd );
                connected.push_back( sck );
            }
            else
            {
                for ( auto it = connected.begin(); it != connected.end(); ++it )
                {
                    if( FD_ISSET( it->get_socket(), &fds ) )
                    {
                        int amount = it->recv();
                        if( amount >= 0 )
                        {
                            std::string received( it->get_buffer( amount ) );
                            it->clear_buffer();
                            Work w { *it, received };
                            pool.add_work( std::move( w ) );
                        }
                        else if( amount == 0 )  // connection is closed
                        {
                            connected.erase( it );
                        }
                    }
                }
            }
        }
    }
}
