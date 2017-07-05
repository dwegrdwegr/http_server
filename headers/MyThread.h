#include <pthread.h>
#include "ThreadSafeQueue.h"
#include "Work.h"
#ifndef _MYTHREAD_H
#define _MYTHREAD_H

class MyBasicThread
{
public:
    MyBasicThread( );
    int join( ) noexcept;
    int cancel( ) noexcept;
    void create( );
private:
    virtual void run( ) = 0;
    static void* start( void* arg );
    pthread_t pthread;
};

class MyThread : public MyBasicThread
{
public:
    explicit MyThread( ThreadSafeQueue* q );
private:
    void run( ) override;

    ThreadSafeQueue* work_queue;
};

class MyRegistrationThread : public MyBasicThread
{
public:
    explicit MyRegistrationThread( MyServerSocket* s );
private:
    void run( ) override;
    void register_users( const std::string& json );

    MyServerSocket* socket;
};
#endif
