#include <string>
#include <map>
#include <utility>
#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H

class HttpResponse
{
        std::string content;
        std::string http_version;
        uint16_t status_code;
        std::string status_desc;
        std::map<std::string, std::string> headers;
    public:
        HttpResponse& set_content( const std::string& content );

        HttpResponse& set_http_version( const std::string& http_version );

        HttpResponse& set_status_code( const uint16_t& status_code );

        HttpResponse& set_status_desc( const std::string& status_desc );

        HttpResponse& append_header( const std::pair< std::string, std::string >& pair );
};

#endif // _HTTP_RESPONSE_H
