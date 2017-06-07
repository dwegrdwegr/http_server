#include "HttpResponse.h"


HttpResponse& HttpResponse::set_content( const std::string& content )
{
    this->content = content;
    return *this;
}
HttpResponse& HttpResponse::set_http_version( const std::string& http_version )
{
    this->http_version = http_version;
    return *this;
}
HttpResponse& HttpResponse::set_status_code( const uint16_t& status_code )
{
    this->status_code = status_code;
    return *this;
}
HttpResponse& HttpResponse::set_status_desc( const std::string& status_desc )
{
    this->status_desc = status_desc;
    return *this;
}
HttpResponse& HttpResponse::append_header( const std::pair< std::string, std::string >& pair )
{
    this->headers.insert( pair );
    return *this;
}
