#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <pthread.h>
#ifndef MYUTILITY_H
#define MYUTILITY_H

struct MySession
{
    std::string id;
    std::string username;
    std::chrono::time_point<std::chrono::system_clock> time;
    MySession( const std::string& id, const std::string& username );
};

class MyUtility
{
public:
    MyUtility( const MyUtility& orig ) = delete;
    MyUtility& operator=(const MyUtility& orig) = delete;

    static MyUtility& get_utility( );
    int generate_session_id( );
    void check_timed_out_sessions( );
    void add_session( MySession s );
    const MySession* get_session( std::string& cookie ) const;
    std::map<std::string, std::string> users{
        { "admin", "password" },
        { "dawid", "dawid1" },
        { "user", "pass" } };
    constexpr static int timeout = 600; // timeout in seconds
private:
    MyUtility( );
    std::vector<MySession> sessions;

    int id = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
};


#endif /* MYUTILITY_H */

