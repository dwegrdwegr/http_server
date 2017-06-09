#include <string>
#include <map>
#include <utility>
#include "HttpResponse.h"
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
    virtual HttpResponse process() = 0;
};

class HttpRequestGET: public HttpRequest
{
        virtual HttpResponse process();
};

class HttpRequestPOST: public HttpRequest
{
        virtual HttpResponse process();
};
#endif // _HTTP_REQUEST_H
