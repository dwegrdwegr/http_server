#include "HttpParser.h"
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

std::unique_ptr<HttpRequest> HttpParser::parse( const std::string& request )
{
    std::vector< std::string > temporary;   // holds all the lines
    std::stringstream str( request );
    std::string item;   // buffer 1
    std::string item2;  // buffer 2

    while ( std::getline( str, item ) )
    {
        temporary.push_back( item );
    }
    str.str( temporary[0] );    // reuse existing stringstream instead of creating a new one
    str.clear();

    std::vector< std::string > second_temporary;    // holds only the first line elements
    while ( str >> item )
    {
        second_temporary.push_back( item );
    }
    std::unique_ptr<HttpRequest> req;
    if( second_temporary[0] == "GET" )
    {
        req = std::make_unique<HttpRequestGET>();
        req->method = method_type::GET;
    }
    else if( second_temporary[0] == "POST" )
    {
        req = std::make_unique<HttpRequestPOST>();
        req->method = method_type::POST;
    }
    else
    {
        throw std::bad_exception();
    }
    req->url = second_temporary[1];
    req->http_version = second_temporary[2];
    size_t i = 1;
    for( ; i < temporary.size(); i++ )
    {
        if( temporary[i] == "\r" || temporary[i] == "" )
        {
            break;
        }
        str.str( temporary[i] );    // reuse existing stringstream instead of creating a new one
        str.clear();
        std::getline( str, item, ':' );
        std::getline( str, item2 );
        if ( !item2.empty() && item2[item2.size() - 1] == '\r' )
        {
            item2.pop_back();
        }
        if ( !item2.empty() && item2[0] == ' ' )
        {
            item2.erase( 0, 1 );
        }
        req->headers[item]=item2;
    }
    ++i;
    for ( ; i< temporary.size(); i++ )
    {
        req->content.append( temporary[i] );
    }
    return req;
}
