#include <string>
#include <iostream>
#include "jsmn.h"

#include "MyServerSocket.h"

int main( int argc, char** argv )
{
    MyServerSocket socket( static_cast < unsigned short > ( 1337 ) );
    std::string json;
    if ( socket.connect( ) == -1 )
        return 1;
    while ( true )
    {
        std::cout << "1. Add users\n2. Remove users\n3. Show users\n4. End\n";
        int option;
        std::cin >> option;
        if ( option == 1 ) // Add user
        {
            int i = 0;
            std::cout << "Number of users to add? Max 10\n";
            do
            {
                std::cin >> i;
            }
            while ( i > 10 || i < 0 );
            json = "{ ";
            json.append( "\"request\" : \"add-users\" ,\"users\" : [" );

            for ( int count = 1; count <= i; count++ )
            {
                std::string username;
                std::string password;

                std::cout << count << ". Username: ";
                std::cin >> username;
                std::cout << "\tPassword: ";
                std::cin >> password;
                std::cout << "\n";
                std::string object{ "{ \"username\" : " + username + " , \"password\" : " + password + " }" };
                json.append( object );
            }
            json.append( " ] }" );
            socket.send( json );
            json.clear( );
            int amount = socket.recv( );
            if ( amount > 0 )
            {
                std::string received( socket.get_buffer( amount ) );
                socket.clear_buffer( );
                jsmn_parser parser;
                jsmn_init( &parser );
                jsmntok_t tokens[55];
                jsmn_parse( &parser, received.c_str( ), received.length( ), tokens, 55 );
                for ( int x = 0; x < i; x++ )
                {
                    std::cout
                            << received.substr( tokens[4 + ( x * 3 )].start, tokens[4 + ( x * 3 )].end - tokens[4 + ( x * 3 )].start )
                            << " "
                            << received.substr( tokens[5 + ( x * 3 )].start, tokens[5 + ( x * 3 )].end - tokens[5 + ( x * 3 )].start )
                            << "\n";
                }
            }
        }
        else if ( option == 2 ) // Remove users
        {
            int i = 0;
            std::cout << "Number of users to remove? Max 10\n";
            do
            {
                std::cin >> i;
            }
            while ( i > 10 || i < 0 );
            json = "{ ";
            json.append( "\"request\" : \"remove-users\" , \"users\" : [" );

            for ( int count = 1; count <= i; count++ )
            {
                std::string username;

                std::cout << count << ". Username: ";
                std::cin >> username;
                std::cout << "\n";
                json.append( "\"" + username + "\", " );
            }
            json.append( " ] }" );
            socket.send( json );
            json.clear( );
            int amount = socket.recv( );
            if ( amount > 0 )
            {
                std::string received( socket.get_buffer( amount ) );
                socket.clear_buffer( );
                jsmn_parser parser;
                jsmn_init( &parser );
                jsmntok_t tokens[55];
                jsmn_parse( &parser, received.c_str( ), received.length( ), tokens, 55 );
                for ( int x = 0; x < i; x++ )
                {
                    std::cout
                            << received.substr( tokens[4 + ( x * 3 )].start, tokens[4 + ( x * 3 )].end - tokens[4 + ( x * 3 )].start )
                            << " "
                            << received.substr( tokens[5 + ( x * 3 )].start, tokens[5 + ( x * 3 )].end - tokens[5 + ( x * 3 )].start )
                            << "\n";
                }
            }
        }
        else if ( option == 3 ) // Show users
        {
            json = "{ \"request\" : \"show-users\" }";
            socket.send( json );
            json.clear( );
            int amount = socket.recv( );
            if ( amount > 0 )
            {
                std::string received( socket.get_buffer( amount ) );
                socket.clear_buffer( );
                jsmn_parser parser;
                jsmn_init( &parser );
                jsmntok_t tokens[55];
                jsmn_parse( &parser, received.c_str( ), received.length( ), tokens, 55 );
                for ( int x = 0; x < tokens[2].size; x++ )
                {
                    std::cout
                            << received.substr( tokens[3 + x].start, tokens[3 + x].end - tokens[3 + x].start )
                            << "\n";
                }
            }
        }
        else if ( option == 4 )
        {
            socket.close();
            break;
        }
        else
        {
            std::cout << "Wrong option picked. Pick 1, 2, 3\n";
        }
    }
    return 0;
}

