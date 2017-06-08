#include <string>
#include "MyServerSocket.h"
#ifndef _WORK_H
#define _WORK_H

struct Work
{
    MyServerSocket socket_fd;
    std::string http_string;
};

#endif //_WORK_H
