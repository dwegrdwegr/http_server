#include <string>
#include <iostream>

#include "MyServerSocket.h"

int main( int argc, char** argv )
{
    MyServerSocket socket(static_cast<unsigned short>(1337));
    std::string json {"{ "};
    while ( true )
    {
        std::cout << "1. Add user\n2. Add users\n";
        int option;
        std::cin>>option;
        if ( option == 1 )
        {
            std::string username;
            std::string password;
            
            std::cout<<"Username: ";
            std::cin>>username;
            std::cout<<"Password: ";
            std::cin>>password;
            
        }
        else if ( option == 2 )
        {

        }
        else
        {
            std::cout << "Wrong option picked. Pick 1 or 2\n";
        }
    }
    return 0;
}

