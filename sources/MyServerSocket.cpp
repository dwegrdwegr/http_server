#include "MyServerSocket.h"
#include <exception>
#include <errno.h>
#include <algorithm>
#include <arpa/inet.h>

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
MyServerSocket::MyServerSocket( int32_t fd )
{
    socket_fd = fd;
    int opt = 1;
    int optlen = sizeof( opt );
    setsockopt( socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, optlen );
}
MyServerSocket::MyServerSocket( const MyServerSocket& second )
{
    this->addr.sin6_family = AF_INET6;
    this->addr.sin6_flowinfo = 0;
    this->addr.sin6_addr = IN6ADDR_ANY_INIT;
    this->addr.sin6_port = htons ( second.port );
    this->port = second.port;
    this->socket_fd = second.socket_fd;
    for( int i = 0; i < 1000; i++ )
    {
        this->buffer[i] = second.buffer[i];
    }
}

MyServerSocket::~MyServerSocket()
{
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
    int result = ::listen( socket_fd, n );
    if ( result != 0 )
    {
        throw std::bad_exception();
    }
}
int MyServerSocket::accept()
{
    return ::accept( socket_fd, nullptr, nullptr );
}
int MyServerSocket::send( const std::string& msg )
{
    return ::send( socket_fd, ( void * ) msg.c_str(), msg.length(), 0 );
}
int MyServerSocket::recv()
{
    return ::recv( socket_fd, buffer, 1000, 0 );
}
void MyServerSocket::close()
{
    ( void ) ::close( socket_fd );
}

int MyServerSocket::connect( )
{
    addr.sin6_family = AF_INET6;
    addr.sin6_flowinfo = 0;
    (void) inet_pton(AF_INET6, "::1",(struct sockaddr *) &addr.sin6_addr);
    addr.sin6_port = htons ( port );
    int result = ::connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr));
    if ( result == -1 )
    {
        printf("Error. Can't connect\n");
    }
    return result;
}

std::string MyServerSocket::get_buffer( int n ) const noexcept
{
    return std::string( buffer, n );
}
void MyServerSocket::clear_buffer()
{
    std::fill( buffer, buffer + 1000, 0 );
}
const int32_t& MyServerSocket::get_socket() const noexcept
{
    return socket_fd;
}
