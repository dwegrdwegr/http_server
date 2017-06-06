#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#ifndef MYSERVERSOCKET_H
#define MYSERVERSOCKET_H

class MyServerSocket
{
    public:
        MyServerSocket( uint16_t port, int domain = AF_INET6, int type = SOCK_STREAM, int protocol = 0 );
        ~MyServerSocket();
        void bind();
        void listen( int n );
        int accept();
    private:
        int32_t socket_fd;
        uint16_t port;
        sockaddr_in6 addr;
};

#endif // MYSERVERSOCKET_H
