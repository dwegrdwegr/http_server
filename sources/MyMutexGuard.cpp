#include "MyMutexGuard.h"

MyMutexGuard::MyMutexGuard( pthread_mutex_t& m ):mutex(m)
{
    pthread_mutex_lock(&mutex);
}

MyMutexGuard::~MyMutexGuard( )
{   
    pthread_mutex_unlock(&mutex);
}

