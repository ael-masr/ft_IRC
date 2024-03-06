#include "Server.hpp"

Server::Server(int port, std::string password)
{
    this->_port = port;
    this->_password = password;
    if ((_port < 1) || (_port > 1000))
        throw ServerException("Invalid port number");
    if ((_password.length() < 1) || (_password.length() > 8))
        throw ServerException("Invalid password length");
    std::cout << "Waiting for connections..." << std::endl;
}
