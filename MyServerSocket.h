#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#ifndef MYSERVERSOCKET_H
#define MYSERVERSOCKET_H

class MyServerSocket
{
    public:
        MyServerSocket( uint16_t port, int domain = AF_INET6, int type = SOCK_STREAM, int protocol = 0 );
        MyServerSocket( int32_t fd );
        MyServerSocket( const MyServerSocket& second );
        ~MyServerSocket();
        void bind();
        void listen( int n );
        int accept();
        void send( const std::string& msg );
        int recv();
        void close();
        std::string get_buffer( int n ) const;
        void clear_buffer();
        const int32_t& get_socket() const noexcept;
    private:
        int32_t socket_fd;
        uint16_t port;
        sockaddr_in6 addr;
        char buffer[1000];
};

#endif // MYSERVERSOCKET_H
