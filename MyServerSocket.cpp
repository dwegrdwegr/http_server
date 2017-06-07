#include "MyServerSocket.h"
#include <exception>

MyServerSocket::MyServerSocket( uint16_t p, int domain , int type, int protocol ) : port { p }
{
    socket_fd = socket( domain, type, protocol );
    if( socket_fd == -1 )
    {
        throw std::bad_exception();
    }
    int opt = 1;
    int optlen = sizeof( opt );
    setsockopt( socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, optlen );
}

MyServerSocket::~MyServerSocket()
{
    close( socket_fd );
}

void MyServerSocket::bind()
{
    addr.sin6_family = AF_INET6;
    addr.sin6_flowinfo = 0;
    addr.sin6_addr = IN6ADDR_ANY_INIT;
    addr.sin6_port = htons ( port );
    int result = ::bind( socket_fd, reinterpret_cast<sockaddr *>( &addr ), sizeof( addr ) );
    if( result < 0 )
    {
        throw std::bad_exception();
    }
}
void MyServerSocket::listen( int n )
{
    ::listen( socket_fd, n );
}
int MyServerSocket::accept()
{
    return ::accept( socket_fd, nullptr, nullptr );
}
