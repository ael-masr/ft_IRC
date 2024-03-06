
#include "Channel.hpp"

User::User()
{
    is_registered = false;
}
/*###################################################################*/

User::User(std::string nickname)
{
    this->nickname = nickname;
    is_registered = false;
}
/*###################################################################*/


std::string User::getNickname() const
{
    return nickname;
}
/*###################################################################*/

void User::setNickname(std::string nickname)
{
    this->nickname = nickname;
}
/*###################################################################*/

User::~User()
{
    // Destructor
}
/*###################################################################*/


bool isNicknameInUse(User user, const std::string& nickname)
{
    for (std::vector<User>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}
/*###################################################################*/
bool ChannelExist(const std::string& channel)
{
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->get_channel_name() == channel) {
            return true;
        }
    }
    return false;
}
/*###################################################################*/

void printWelcome( User& user)
{
    std::cout << "001 Welcome to the Internet Relay Network " << user.getNickname() << std::endl;
    std::cout << "002 Your host is mock.server, running version 1.0" << std::endl;
    std::cout << "003 This server was created <date>" << std::endl;
    std::cout << "004 mock.server 1.0 ao mtov" << std::endl; // Example modes
}
/*###################################################################*/

void printError( const std::string& errorType,const std::string& message)
{
    std::cout << "Error " << errorType << ": " << message << std::endl;
}
/*###################################################################*/

bool isNumeric(const std::string& str)
{
    // Check if the string is empty or if it's a negative number
    if(str.empty())
        std::cout << "empty input" << std::endl;
    for(std::string::size_type i = 1; i < str.size(); ++i)
    {
        if(!std::isdigit(str[i]))
            return false;
    }
        return true;
}
/*###################################################################*/

std::vector<std::string> split(const std::string str)
{
	std::vector<std::string> vector;
	std::istringstream iss(str);
	std::string cmd;
	while (iss >> std::skipws >> cmd)
		vector.push_back(cmd);
	return vector;
}
/*###################################################################*/


void    handleNickCommand(std::vector<std::string>& parameters,User& currentUser)
{
    if (parameters.size() == 0)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
    {
        if (isNicknameInUse(currentUser, parameters[0]))
            std::cout << "Nickname is already in use" << std::endl;
        else
            currentUser.setNickname(parameters[0]);
    }
}
/*###################################################################*/
bool    isUserInChannel(const std::string& nickname, const std::string& channel)
{
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->get_channel_name() == channel)
        {
            for (std::vector<User>::const_iterator it2 = it->get_users().begin(); it2 != it->get_users().end(); ++it2)
            {
                if (it2->getNickname() == nickname)
                    return true;
            }
        }
    }
    return false;
}
/*###################################################################*/
void    kickUserFromChannel(const std::string& nickname, const std::string& channel)
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->get_channel_name() == channel)
        {
            for (std::vector<User>::iterator it2 = it->get_users().begin(); it2 != it->get_users().end(); ++it2)
            {
                if (it2->getNickname() == nickname)
                {
            //        it->removeUser(nickname); /*To be done*/
                    return;
                }
            }
        }
    }
}

/*###################################################################*/
void    handleKickCommand(std::vector<std::string>& parameters)
{
    if (parameters.size()  != 2)
        std::cout << "461 :Not enough parameters" << std::endl;
    else if(!ChannelExist(parameters[0]))
        std::cout << "ERR_NOSUCHCHANNEL_C :No such channel" << std::endl;
    else if(!isUserInChannel(parameters[1], parameters[0]))
        std::cout << "ERR_USERNOTINCHANNEL_C :They aren't on that channel" << std::endl;
    else kickUserFromChannel(parameters[1], parameters[0]);

}

/*###################################################################*/
void    handlePrivmsgCommand(std::vector<std::string> parameters)
{
    if (parameters.size()  != 2)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
        std::cout << "PRIVMSG " << parameters[0] << " " << parameters[1] << std::endl;
}

/*###################################################################*/
void    handleJoinCommand(std::vector<std::string>& parameters)
{
    if (parameters.size()  != 1)
        std::cout << "461 :Not enough parameters" << std::endl;
    else channels.push_back(Channel(parameters[0], '#'));
}

/*###################################################################*/
void    handlePartCommand(std::vector<std::string>& parameters)
{
    /*To be done*/

}

/*###################################################################*/
void    handleModeCommand(std::vector<std::string>& parameters)
{
    if (parameters.size()  != 2)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
        std::cout << "MODE " << parameters[0] << " " << parameters[1] << std::endl;
}
/*###################################################################*/
void    dispatchCommands(const std::string& command, std::vector<std::string>& parameters, User& currentUser)
{
    if (command == "NICK")
        handleNickCommand(parameters, currentUser);
    else if (command == "KICK")
        handleKickCommand(parameters);
    else if (command == "PRIVMSG")
        handlePrivmsgCommand(parameters);
    else if (command == "JOIN")
        handleJoinCommand(parameters);
    else if (command == "PART")
        handlePartCommand(parameters);
    else if (command == "MODE")
        handleModeCommand(parameters);
    else
        std::cout << "421 :Unknown command" << std::endl;
}
/*###################################################################*/

void parse(User currentUser)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter command: ";
        if (!std::getline(std::cin, input) || input == "QUIT")
        {
            std::cout << "Disconnecting user and exiting program.\n";
            break;
        }
        std::istringstream iss(input);
        std::string command;
        std::vector<std::string> parameters;
        iss >> command; 
        std::string param;
        while (iss >> param && parameters.size() < 2)
        {
            parameters.push_back(param);
        }
        if(command == "NICK" ||  command == "KICK"
            || command == "PRIVMSG" || command == "JOIN" || command == "PART" || command == "MODE")
        dispatchCommands(command, parameters, currentUser);
        else
            std::cout << "421 :Unknown command" << std::endl;
    }
}

/*###################################################################*/

int main(int argc, char **argv)
{
    if (argc == 3 && isNumeric(argv[1]))
    {
        User currentUser;
        users.push_back(currentUser);
        Server Server(std::atoi(argv[1]), argv[2]);
        parse(currentUser);
    }
    return 0;
}