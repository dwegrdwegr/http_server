#include <string>
#include <memory>
#include "HttpRequests.h"
#ifndef _HTTP_PARSER_H
#define _HTTP_PARSER_H

class HttpParser
{
    public:
        std::unique_ptr<HttpRequest> parse( const std::string& request );
};

#endif // _HTTP_PARSER_H
