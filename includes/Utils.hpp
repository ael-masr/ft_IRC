#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Server.hpp"

using namespace std;

class User;
class Channel;

namespace Utils {

	// Check if a user exists 
	User	find_User(User nickname);
	User	find_User(string nickname);
	Channel	find_Channel(string channel);
	int		find_Channel(Channel channel);
	bool	username_exists(string username);
	bool	nickname_exists(string nickname);
	void	sendErrorMessage(int fd, const std::string& message, const int key);
	template <typename T>
	int binarySearch(std::vector<T> users, T nickname, unsigned int size);
    // std::string to_string(int value);
	// std::vector<std::string> split(const std::string str);
	// std::string trim(std::string &str);
	void signalHandler(int signum);
	User &find(int fd);
	// void closeThis(User &user);
}

# endif // UTILS_HPP
