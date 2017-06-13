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
    return str;
}
