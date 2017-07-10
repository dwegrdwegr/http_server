#include <map>
#include <pthread.h>
#include <MyMutexGuard.h>
#ifndef THREADSAFEMAP_H
#define THREADSAFEMAP_H

template <typename key, typename value>
class ThreadSafeMap
{
    typedef typename std::map<key, value>::iterator iterator;
public:

    ThreadSafeMap( ) : mutex( PTHREAD_MUTEX_INITIALIZER )
    {
    };

    iterator find( key to_find )
    {
        MyMutexGuard guard( mutex );
        return _map.find( to_find );
    }

    iterator begin( )
    {
        return _map.begin( );
    }

    iterator end( )
    {
        return _map.end( );
    }

    value& operator[](key arg)
    {
        MyMutexGuard guard( mutex );
        return _map[arg];
    }

    iterator erase( iterator input )
    {
        return _map.erase( input );
    }
private:
    std::map<key, value> _map;
    pthread_mutex_t mutex;
};

#endif /* THREADSAFEMAP_H */

