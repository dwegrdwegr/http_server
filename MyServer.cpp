#include "MyServer.h"

MyServer::MyServer(): socket( 8080 )
{

}

MyServer::~MyServer()
{
}
void MyServer::run()
{
    socket.bind();
    socket.listen( 10 );
    // to be continued ...
}
