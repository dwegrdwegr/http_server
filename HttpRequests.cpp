#include "HttpRequests.h"
#include "htmls.h"

HttpResponse HttpRequestGET::process()
{
    HttpResponse response;
    if( url == "/login.html" )
    {
        response.http_version = "HTTP/1.1";
        response.status_code = "200";
        response.status_desc = "OK";
        response.headers["Content-Type"] = "text/html; charset=UTF-8";
        response.headers["Content-Length"] = login_html.length();
        response.content = login_html;
    }
    else
    {
        response.http_version = "HTTP/1.1";
        response.status_code = "404" ;
        response.status_desc = "Not found";
    }
    return response;
}
HttpResponse HttpRequestPOST::process()
{
    if( url == "/login.html" )
    {

    }
    else
    {

    }
}
