#include "HttpRequests.h"
#include "htmls.h"
#include "MyMutexGuard.h"
#include "MyUtility.h"
#include <sstream>
#include <iostream>

HttpResponse HttpRequestGET::process( )
{
    MyUtility::get_utility( ).check_timed_out_sessions( );
    HttpResponse response;
    if ( url == "/login.html" )
    {
        auto cookie = headers.find( "Cookie" );

        if ( cookie != headers.end( ) )
        {
            const MySession* it = MyUtility::get_utility( ).get_session( cookie->second );
            if ( it == NULL )
            {
                response.http_version = "HTTP/1.1";
                response.status_code = "200";
                response.status_desc = "OK";
                response.headers["Content-Type"] = "text/html; charset=UTF-8";
                response.content = login_html;
                response.replace_content_substring( "{error}", "" );
                response.headers["Content-Length"] = std::to_string( response.content.length( ) );
            }
            else
            {
                std::string username = it->username;
                response.http_version = "HTTP/1.1";
                response.status_code = "200";
                response.status_desc = "OK";
                response.headers["Content-Type"] = "text/html; charset=UTF-8";
                response.content = hello_html;
                response.replace_content_substring( "{username}", username );
                response.headers["Content-Length"] = std::to_string( response.content.length( ) );
            }
        }
        else
        {
            response.http_version = "HTTP/1.1";
            response.status_code = "200";
            response.status_desc = "OK";
            response.headers["Content-Type"] = "text/html; charset=UTF-8";
            response.content = login_html;
            response.replace_content_substring( "{error}", "" );
            response.headers["Content-Length"] = std::to_string( response.content.length( ) );
        }
    }
    else
    {
        response.http_version = "HTTP/1.1";
        response.status_code = "404";
        response.status_desc = "Not found";
    }
    return response;
}

HttpResponse HttpRequestPOST::process( )
{
    MyUtility::get_utility( ).check_timed_out_sessions( );
    HttpResponse response;
    if ( url == "/login.html" )
    {
        std::stringstream stream( content );
        std::string item;
        std::map< std::string, std::string > post;
        while ( std::getline( stream, item, '&' ) )
        {
            std::string key;
            std::string value;
            std::stringstream item_stream( item );
            std::getline( item_stream, key, '=' );
            std::getline( item_stream, value );
            post[key] = value;
        }
        auto it = post.find( "username" );
        if ( it == post.end( ) ) // didnt find username in post
        {
            response.http_version = "HTTP/1.1";
            response.status_code = "400";
            response.status_desc = "Bad request";
        }
        else
        {
            auto user = MyUtility::get_utility( ).users.find( it->second );
            if ( user == MyUtility::get_utility( ).users.end( ) ) // user not found
            {
                response.http_version = "HTTP/1.1";
                response.status_code = "400";
                response.status_desc = "User not found";
                response.headers["Content-Type"] = "text/html; charset=UTF-8";
                response.content = login_html;
                response.replace_content_substring( "{error}", "User not found" );
                response.headers["Content-Length"] = std::to_string( response.content.length( ) );
            }
            else
            {
                if ( MyUtility::get_utility( ).users.find( it->second )->second == post.find( "password" )->second )
                {
                    response.http_version = "HTTP/1.1";
                    response.status_code = "200";
                    response.status_desc = "OK";
                    response.headers["Content-Type"] = "text/html; charset=UTF-8";
                    response.content = hello_html;
                    response.replace_content_substring( "{username}", it->second );
                    response.headers["Content-Length"] = std::to_string( response.content.length( ) );
                    std::string session_id = std::to_string( MyUtility::get_utility( ).generate_session_id( ) );
                    response.headers["Set-Cookie"] = "id=" + session_id + "; Max-Age=" + std::to_string( MyUtility::timeout );
                    MySession s( session_id, it->second );
                    MyUtility::get_utility( ).add_session( s );
                }
                else
                {
                    response.http_version = "HTTP/1.1";
                    response.status_code = "400";
                    response.status_desc = "Invalid password";
                    response.headers["Content-Type"] = "text/html; charset=UTF-8";
                    response.content = login_html;
                    response.replace_content_substring( "{error}", "Invalid password" );
                    response.headers["Content-Length"] = std::to_string( response.content.length( ) );
                }
            }
        }

    }
    else
    {
        response.http_version = "HTTP/1.1";
        response.status_code = "404";
        response.status_desc = "Not found";
    }
    return response;
}
