#include<pthread>
#ifndef _MYTHREAD
#define _MYTHREAD

class MyThread
{
private:
    pthread_t pthread;
    bool execute;
}
#endif