#include <string>
#include <map>
#include <utility>
#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H

struct HttpResponse
{
        std::string content;
        std::string http_version;
        std::string status_code;
        std::string status_desc;
        
        std::map<std::string, std::string> headers;

        std::string to_string();
        void replace_substring( std::string& str, const std::string& what, const std::string& value );
};

#endif // _HTTP_RESPONSE_H
