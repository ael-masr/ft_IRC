#ifndef USER_HPP
#define USER_HPP

// #include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
// #include <alogrithm>
#include <cstring>

// User error messages (RFC 2812 section 5.2)
#define ERR_ALREADYREGISTRED_M      " :Unauthorized command (already registered)\n" // 462		Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).

// User error codes (RFC 2812 section 5.2)
#define ERR_ALREADYREGISTRED_C      462	// " :Unauthorized command (already registered)\n"	    Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).

using namespace std;
class Channel;

class User
{
    public:
        // CONSTRUCTORS
        User();
        User(string username, string nickname);
        ~User();

        // OVERLOAD
        bool    operator==(User const &rhs);

        // METHODS
        static bool    user_exists(string nickname);
        static bool    user_exists(User nickname);

        // GETTERS
        vector<Channel> getChannels() const;
        string          getUsername() const;
        string          getNickname() const;
        int             getFd() const;
        bool            isRegistered() const;

        // SETTERS
        void    setUsername(string username);
        void    setNickname(string nickname);
        void    setRegistered(bool isRegistered);
        void    setFd(int fd);
        void    addChannel(Channel channel);
        void    removeChannel(Channel channel);
        
    private:
        vector<Channel> channels_;
        string  username_;
        string  nickname_;
        bool    isRegistered_;
        int     fd_;

};

# endif // USER_HPP
