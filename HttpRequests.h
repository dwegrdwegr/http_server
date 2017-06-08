#include <string>
#include <map>
#include <utility>
#include "MyServerSocket.h"
#ifndef _HTTP_REQUEST_H
#define _HTTP_REQUEST_H
enum class method_type
{
    GET, POST
};

struct HttpRequest
{
    method_type method;
    std::string url;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::string content;
    virtual void process( MyServerSocket socket ) = 0;
};

class HttpRequestGET: public HttpRequest
{
        virtual void process( MyServerSocket socket );
};

class HttpRequestPOST: public HttpRequest
{
        virtual void process( MyServerSocket socket );
};
#endif // _HTTP_REQUEST_H
