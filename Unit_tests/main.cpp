#include <unittest++/UnitTest++.h>
#include "../Work.h"
#include "../HttpParser.h"
#include "../HttpRequests.h"
#include "../htmls.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To add a test, simply put the following code in the a .cpp file of your choice:
//
// =================================
// Simple Test
// =================================
//
//  TEST(YourTestName)
//  {
//  }
//
// The TEST macro contains enough machinery to turn this slightly odd-looking syntax into legal C++, and automatically register the test in a global list.
// This test list forms the basis of what is executed by RunAllTests().
//
// If you want to re-use a set of test data for more than one test, or provide setup/teardown for tests,
// you can use the TEST_FIXTURE macro instead. The macro requires that you pass it a class name that it will instantiate, so any setup and teardown code should be in its constructor and destructor.
//
//  struct SomeFixture
//  {
//    SomeFixture() { /* some setup */ }
//    ~SomeFixture() { /* some teardown */ }
//
//    int testData;
//  };
//
//  TEST_FIXTURE(SomeFixture, YourTestName)
//  {
//    int temp = testData;
//  }
//
// =================================
// Test Suites
// =================================
//
// Tests can be grouped into suites, using the SUITE macro. A suite serves as a namespace for test names, so that the same test name can be used in two difference contexts.
//
//  SUITE(YourSuiteName)
//  {
//    TEST(YourTestName)
//    {
//    }
//
//    TEST(YourOtherTestName)
//    {
//    }
//  }
//
// This will place the tests into a C++ namespace called YourSuiteName, and make the suite name available to UnitTest++.
// RunAllTests() can be called for a specific suite name, so you can use this to build named groups of tests to be run together.
// Note how members of the fixture are used as if they are a part of the test, since the macro-generated test class derives from the provided fixture class.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// run all tests


SUITE( Http )
{
    class Work_fixture
    {
        public:
            Work work;
            HttpParser parser;
            Work_fixture(): work {1}
            {
                work.http_string = "GET /login.html HTTP/1.1\r\n"
                "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                "Host: www.tutorialspoint.com\r\n"
                "Accept-Language: en-us\r\n"
                "Accept-Encoding: gzip, deflate\r\n"
                "Connection: Keep-Alive\r\n"
                "\r\n"
                "Message body";
            }
            ~Work_fixture() {};
    };
    class Work_fixture2
    {
        public:
            Work work;
            HttpParser parser;
            Work_fixture2(): work {1}
            {
                work.http_string = "GET /hello.html HTTP/1.1\r\n"
                "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                "Host: www.tutorialspoint.com\r\n"
                "Accept-Language: en-us\r\n"
                "Accept-Encoding: gzip, deflate\r\n"
                "Connection: Keep-Alive\r\n"
                "\r\n"
                "Message body";
            }
            ~Work_fixture2() {};
    };
    TEST_FIXTURE( Work_fixture, ParsingHTTP )
    {
        std::unique_ptr<HttpRequest> req = parser.parse( work.http_string );
        CHECK( req->method == method_type::GET );
        CHECK( req->url == "/login.html" );
        CHECK( req->http_version == "HTTP/1.1" );
        CHECK( req->headers.find( "User-Agent" )->second == "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)" );
        CHECK( req->headers.find( "Host" )->second == "www.tutorialspoint.com" );
        CHECK( req->headers.find( "Accept-Language" )->second == "en-us" );
        CHECK( req->headers.find( "Accept-Encoding" )->second == "gzip, deflate" );
        CHECK( req->headers.find( "Connection" )->second == "Keep-Alive" );
        CHECK( req->content == "Message body" );
    }
    TEST_FIXTURE( Work_fixture, ResponseHTTP )
    {
        std::unique_ptr<HttpRequest> req = parser.parse( work.http_string );
        HttpResponse res = req->process();
        CHECK( res.http_version == "HTTP/1.1" );
        CHECK( res.status_code == "200" );
        CHECK( res.status_desc == "OK" );
        CHECK( res.content == login_html );
    }
    TEST_FIXTURE( Work_fixture2, WrongRequestResponseHTTP )
    {
        std::unique_ptr<HttpRequest> req = parser.parse( work.http_string );
        HttpResponse res = req->process();
        CHECK( res.http_version == "HTTP/1.1" );
        CHECK( res.status_code == "404" );
        CHECK( res.status_desc == "Not found");
    }
}
int main( int argc, char **argv )
{
    return UnitTest::RunAllTests();
}
