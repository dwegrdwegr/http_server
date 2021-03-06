#include "MyThreadPool.h"
#include "ThreadSafeQueue.h"
#include "MyServerSocket.h"
#ifndef _MYSERVER_H
#define _MYSERVER_H

class MyServer
{
        ThreadSafeQueue work_queue;
        MyThreadPool pool;
        MyServerSocket socket;
        MyServerSocket registration_socket;
        MyJSONThread registration_thread;
    public:
        MyServer();
        ~MyServer();

        void run();
};

#endif // _MYSERVER_H
