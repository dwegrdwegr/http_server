#include <string>
#include "HttpRequest.h"
#ifndef _HTTP_PARSER_H
#define _HTTP_PARSER_H

class HttpParser
{
    public:
        HttpRequest parse( const std::string& request );
};

#endif // _HTTP_PARSER_H
