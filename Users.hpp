#ifndef USERS_HPP
#define USERS_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <sstream>
#include <cctype> // For std::isdigit
using namespace std;
class User
{
    public:
        // CONSTRUCTORS
        User();
        User(string nickname);
        ~User();
        bool is_registered;
        void setNickname(std::string nickname);
        std::string getNickname() const;
        
    private:
        std::string nickname;

};
vector<User> users; // List of connected users
# endif // USERS_HPP