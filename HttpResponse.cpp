#include "HttpResponse.h"

std::string HttpResponse::to_string() const
{
    std::string str;
    str += http_version + " " + status_code + " " + status_desc + "\r\n";
    if( !headers.empty() )
    {
        for( auto it = headers.begin(); it != headers.end(); it++ )
        {
            str += it->first + ": " + it->second + "\r\n";
        }
    }
    str += "\r\n";
    str += content;
    str += "\r\n\r\n";
    return str;
}

void HttpResponse::replace_content_substring( const std::string& what, const std::string& value )
{
    size_t pos =  content.find(what);
    content.erase(pos,what.length());
    content.insert(pos,value);
}

HttpResponse generate_bad_request()
{
    HttpResponse response;
    response.content = "";
    response.http_version = "HTTP/1.1";
    response.status_code = "400";
    response.status_desc = "Bad request";
    return response;
}
