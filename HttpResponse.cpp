#include "HttpResponse.h"

std::string HttpResponse::to_string()
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

void HttpResponse::replace_substring( std::string& str, const std::string& what, const std::string& value )
{
    size_t pos =  str.find(what);
    str.erase(pos,what.length());
    str.insert(pos,value);
}
