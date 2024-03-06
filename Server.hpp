#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <unistd.h>

class Server
{	
public:
	Server(int port, std::string password);
	std::string _password;
	int _port;
};

	class ServerException : public std::exception
	{
		private:
			std::string _msg;
		public:
			ServerException(std::string msg) : _msg(msg) {}
			virtual ~ServerException() throw() {}
			virtual const char *what() const throw() { return _msg.c_str(); }
	};

	static std::vector<int> _fds;
	static void openSocket(void);
	static void run(void);
	static void acceptConnection(void);
	static void handleClientMessages(void);
	static void showUsers(void);
	static void showChannels(void);
	static std::string getPassword(void);
#endif