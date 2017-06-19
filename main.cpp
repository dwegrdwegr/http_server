#include "MyServer.h"
#include "ThreadSafeQueue.h"
#include <pthread.h>

void* start( void *a )
{
    ThreadSafeQueue* xD = static_cast < ThreadSafeQueue* > ( a );
    for ( int i = 0; i < 10000; i++ )
    {
        auto item = xD->pop( );
        printf( "Kebab" );
    }
}

int main( int, char** )
{
    ThreadSafeQueue xD;
    pthread_t t;
    pthread_create( &t, NULL, start, &xD );
    for ( int i = 0; i < 10000; i++ )
        xD.push({ 1, "GET /login.html HTTP/1.1" } );
    
    pthread_join(t, NULL);
    //MyServer server;
    //server.run();
    return 0;
}
