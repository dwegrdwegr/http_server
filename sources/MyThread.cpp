#include "MyThread.h"
#include <fstream>
#include "HttpRequests.h"
#include "HttpResponse.h"
#include "HttpParser.h"
#include <stdio.h>
#include "jsmn.h"
#include <exception>
#include "MyServerSocket.h"
#include "MyUtility.h"
#include <sys/select.h>
#include <vector>

MyBasicThread::MyBasicThread( )
{

}

int MyBasicThread::join( ) noexcept
{
    return pthread_join( pthread, NULL );
}

int MyBasicThread::cancel( ) noexcept
{
    return pthread_cancel( pthread );
}

MyJSONThread::MyJSONThread( MyServerSocket* s )
{
    socket = s;
}

void MyJSONThread::manage_requests( const std::string& json, MyServerSocket& client )
{
    jsmn_parser parser;
    jsmn_init( &parser );
    jsmntok_t tokens[55];
    jsmn_parse( &parser, json.c_str( ), json.length( ), tokens, 55 );

    if ( tokens[1].type == JSMN_STRING
            && json.substr( tokens[1].start, tokens[1].end - tokens[1].start ) == "request"
            && json.substr( tokens[2].start, tokens[2].end - tokens[2].start ) == "add-users" )
    {
        if ( tokens[4].type == JSMN_ARRAY )
        {
            std::string response = "{ \"response\" : [";
            std::ofstream file( "users.txt", std::ios_base::out | std::ios_base::app );

            int number_of_children = tokens[4].size;
            for ( int n = 0; n < number_of_children; n++ )
            {
                response.append( " { " );
                std::string username = json.substr( tokens[7 + ( 5 * n )].start, tokens[7 + ( 5 * n )].end - tokens[7 + ( 5 * n )].start );
                std::string password = json.substr( tokens[9 + ( 5 * n )].start, tokens[9 + ( 5 * n )].end - tokens[9 + ( 5 * n )].start );
                response.append( "\"" + username + "\"" );
                auto found_user = MyUtility::get_utility( ).users.find( username );
                if ( found_user != MyUtility::get_utility( ).users.end( ) )
                {
                    response.append( " : \"already-registered\" " );
                }
                else
                {
                    MyUtility::get_utility( ).users[username] = password;
                    if ( file.is_open( ) )
                    {
                        file << username << " " << password << "\n";
                    }
                    response.append( " : \"registered\" " );
                }
                response.append( " } " );
            }
            response.append( " ] }" );
            client.send( response );
            file.close( );
        }
    }
    else if ( tokens[1].type == JSMN_STRING
            && json.substr( tokens[1].start, tokens[1].end - tokens[1].start ) == "request"
            && json.substr( tokens[2].start, tokens[2].end - tokens[2].start ) == "remove-users" )
    {
        if ( tokens[4].type == JSMN_ARRAY )
        {
            std::string response = "{ \"response\" : [";

            int number_of_children = tokens[4].size;
            for ( int n = 0; n < number_of_children; n++ )
            {
                response.append( " { " );
                std::string username = json.substr( tokens[5 + n].start, tokens[5 + n].end - tokens[5 + n].start );

                response.append( "\"" + username + "\"" );
                auto found_user = MyUtility::get_utility( ).users.find( username );
                if ( found_user != MyUtility::get_utility( ).users.end( ) )
                {
                    MyUtility::get_utility( ).users.erase( found_user );
                    response.append( " : \"removed\" " );
                }
                else
                {
                    response.append( " : \"not-found\" " );
                }
                response.append( " } " );
            }
            response.append( " ] }" );
            std::remove( "users.txt" );
            std::ofstream file( "users.txt", std::ios_base::out | std::ios_base::app );
            auto it = MyUtility::get_utility( ).users.begin( );
            if ( file.is_open( ) )
            {
                while ( it != MyUtility::get_utility( ).users.end( ) )
                {
                    file << it->first << " " << it->second << "\n";
                    ++it;
                }
            }
            file.close( );
            client.send( response );
        }
    }
    else if ( tokens[1].type == JSMN_STRING
            && json.substr( tokens[1].start, tokens[1].end - tokens[1].start ) == "request"
            && json.substr( tokens[2].start, tokens[2].end - tokens[2].start ) == "show-users" )
    {
        std::string response{ " { \"users\" : [ " };
        auto it = MyUtility::get_utility( ).users.begin( );
        while ( it != MyUtility::get_utility( ).users.end( ) )
        {
            response.append( "\"" + it->first + "\" , " );
            ++it;
        }
        response.append( " ] }" );
        client.send( response );
    }
}

void MyJSONThread::run( )
{
    std::vector<MyServerSocket> connected;
    int nfds = socket->get_socket( );
    fd_set fds;
    FD_ZERO( &fds );
    FD_SET( socket->get_socket( ), &fds );
    while ( true )
    {
        for ( auto it = connected.begin( ); it != connected.end( ); ++it )
        {
            FD_SET( it->get_socket( ), &fds );
            if ( it->get_socket( ) >= nfds )
            {
                nfds = it->get_socket( ) + 1;
            }
        }

        if ( select( nfds + 1, &fds, NULL, NULL, NULL ) > 0 )
        {
            if ( FD_ISSET( socket->get_socket( ), &fds ) )
            {
                int fd = socket->accept( );
                MyServerSocket sck( fd );
                connected.push_back( sck );
            }
            else
            {
                auto it = connected.begin( );
                auto end = connected.end( );
                while ( it != end )
                {
                    if ( FD_ISSET( it->get_socket( ), &fds ) )
                    {
                        FD_CLR( it->get_socket( ), &fds );
                        int amount = it->recv( );
                        if ( amount > 0 )
                        {
                            std::string received( it->get_buffer( amount ) );
                            it->clear_buffer( );
                            manage_requests( received, *it );
                            ++it;
                        }
                        else if ( amount == 0 ) // connection is closed
                        {
                            connected.erase( it++ );
                        }
                    }
                }
            }
        }
    }
}

MyThread::MyThread( ThreadSafeQueue* q )
{
    work_queue = q;
}

void MyThread::run( )
{
    HttpParser parser;
    while ( true )
    {
        Work w = work_queue->pop( );
        HttpResponse response;
        try
        {
            std::unique_ptr<HttpRequest> req = parser.parse( w.http_string );
            response = req->process( );
        }
        catch ( std::exception& e )
        {
            response = generate_bad_request( );
        }
        w.socket_fd.send( response.to_string( ) );
    }
}

void MyBasicThread::create( )
{
    int result = pthread_create( &pthread, nullptr, MyBasicThread::start, static_cast < void * > ( this ) );
    if ( result != 0 )
    {
        throw std::bad_exception( );
    }
}

void* MyBasicThread::start( void* arg )
{
    MyBasicThread* this_thread = static_cast < MyBasicThread * > ( arg );
    this_thread->run( );
    return NULL;
}
