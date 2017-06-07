#include "HttpParser.h"
#include <vector>
#include <sstream>

HttpRequest HttpParser::parse( const std::string& request )
{
    std::vector< std::string > temporary;
    std::stringstream str( request );
    std::string item;   // buffer 1
    std::string item2;  // buffer 2
    while ( std::getline( str, item ) )
    {
        temporary.push_back( item );
    }
    str.str( temporary[0] );
    std::vector< std::string > second_temporary( 4 );
    while ( str >> item )
    {
        second_temporary.push_back( item );
    }
    HttpRequest req;
    if( second_temporary[0] == "GET" )
    {
        req.method = method_type::GET;
    }
    else if( second_temporary[0] == "POST" )
    {
        req.method = method_type::POST;
    }
    else
    {
        throw std::bad_exception();
    }
    req.url = second_temporary[1];
    req.http_version = second_temporary[2];
    bool flag = true;
    size_t i = 1;
    for( ; i < temporary.size()  && flag; i++ )
    {
        if( temporary[i] == "\r\n" )
        {
            flag=false;
        }
        str.str( temporary[i] );
        str>>item>>item2;
        req.headers.insert( { item, item2 } );
    }
    for ( ; i< temporary.size(); i++ )
    {
        req.content.append( temporary[i] );
    }
    return req;
}
