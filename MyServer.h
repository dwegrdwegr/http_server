#include "MyThreadPool.h"
#include "MyServerSocket.h"
#ifndef _MYSERVER_H
#define _MYSERVER_H

class MyServer
{
        MyThreadPool pool;
        MyServerSocket socket;
    public:
        MyServer();
        ~MyServer();

        void run();
};

#endif // _MYSERVER_H
