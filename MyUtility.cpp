#include "MyUtility.h"
#include <chrono>
#include <sstream>
#include <iostream>
#include "MyMutexGuard.h"

MySession::MySession( const std::string& id, const std::string& username )
{
    this->id = id;
    this->username = username;
    this->time = std::chrono::system_clock::now( );
}

MyUtility::MyUtility( )
{

}

int MyUtility::generate_session_id( )
{

    static int id = 0;
    MyMutexGuard lock( mutex );
    if ( id == INTMAX_MAX )
    {
        id = 0;
    }
    else
    {
        id++;
    }
    return id;
}

MyUtility& MyUtility::get_utility( )
{
    static MyUtility utility;
    return utility;
}

const MySession* MyUtility::get_session( std::string& cookie ) const
{
    std::stringstream stream( cookie );
    std::string key_pair;
    std::getline( stream, key_pair, '&' );
    stream.clear( );
    stream.str( key_pair );
    std::string key;
    std::string value;
    std::getline( stream, key, '=' );
    std::getline( stream, value );

    for ( size_t i = 0; i < sessions.size( ); i++ )
    {
        if ( sessions[i].id == value )
        {
            return &sessions[i];
        }
    }
    return NULL;
}

void MyUtility::add_session( MySession s )
{
    sessions.push_back( s );
}

void MyUtility::check_timed_out_sessions( )
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now( );
    MyMutexGuard guard(mutex);
    for ( size_t i = 0; i < sessions.size( ); i++ )
    {
        std::chrono::duration<double> dur = sessions[i].time - now;
        if ( dur.count( ) >= timeout )
        {
            
            sessions.erase( sessions.begin( ) + i );
        }
    }    
}
